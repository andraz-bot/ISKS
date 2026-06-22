import * as http from 'http';
import express from 'express';
import ip from 'ip';
import path from 'path';
import { fileURLToPath } from 'url';
import { dirname } from 'path';
import WebSocket, { WebSocketServer } from 'ws';

// Priprava poti za serviranje HTML datoteke
const __dirname = dirname(fileURLToPath(import.meta.url));
const app = express();

// --- KONFIGURACIJA STREŽNIKOV ---
// wss1: Komunikacijski kanal za spletno stran (brskalnik)
const wss1 = new WebSocketServer({ port: 8888 });
// wss2: Komunikacijski kanal za ESP32 mikrokrmilnik
const wss2 = new WebSocketServer({ port: 8811 });

// Osnovni HTTP strežnik za prikaz spletne strani (port 8080)
const server = http.createServer(app).listen(8080);

// Ob prijavi na "/" pošljemo HTML datoteko uporabniku
app.get('/', function(req, res){
    res.sendFile(path.join(__dirname + '/fotoUpor.html'));
});


// Pošiljanje podatkov vsem povezanim brskalnikom (broadcast)
// Funkcija preteče seznam vseh aktivnih WebSocket odjemalcev (brskalnikov) 
// in jim pošlje JSON paket, kar omogoča posodobitev grafov v realnem času.
wss1.broadcast = function(data) {
    wss1.clients.forEach(function(client) {
        if(client.readyState === WebSocket.OPEN) {
            client.send(data);
        }
    });
};

// Pošiljanje ukazov napravam
// Ta funkcija omogoča, da strežnik pošlje ukaze (npr. vklop/izklop LED) na esp32
wss2.broadcast = function(data) {
    wss2.clients.forEach(function(client) {
        if(client.readyState === WebSocket.OPEN) {
            client.send(data);
        }
    });
};

// Upravljanje komunikacije z brskalnikom
// Ko brskalnik pošlje ukaz (npr. "vklopi LED"), strežnik prejme JSON sporočilo, 
// ga interpretira in preko 'wss2.broadcast' posreduje ustrezen ukaz ESP32 modulu.
wss1.on('connection', function(ws) {
    console.log("Brskalnik povezan.");
    ws.on('message', function(msgString) {
        try {
            let msg = JSON.parse(msgString);
            // Če brskalnik pošlje ukaz za LED, ga posredujemo naprej na ESP32 (wss2)
            if (msg.tipSporočila === "LED") {
                console.log("Ukaz za LED: " + msg.vrednost);
                wss2.broadcast(JSON.stringify(msg));
            }
        } catch (e) { console.error("Napaka pri parsiranju JSON iz brskalnika"); }
    });
});

// Upravljanje telemetrije z ESP32 (wss2.on)
// Ko ESP32 pošlje meritev senzorja, strežnik to sporočilo sprejme in 
// ga s funkcijo 'broadcast' takoj posreduje vsem povezanim spletnim odjemalcem.
wss2.on('connection', function(ws) {
    console.log("ESP32 povezan.");
    ws.on('message', function(msgString) {
        try {
            let msg = JSON.parse(msgString);
            // Če ESP32 pošlje podatke o svetlobi, jih posredujemo brskalniku (wss1)
            if (msg.tipSporočila === "fotoupornik") {
                wss1.broadcast(JSON.stringify(msg));
            }
        } catch (e) { console.error("Napaka pri parsiranju JSON iz ESP32"); }
    });
});

// Izpis informacij za zagon sistema v terminalu
console.log('Spletni strežnik: http://' + ip.address() + ':8080');
console.log('WebSocket brskalnik: port 8888');
console.log('WebSocket ESP32: port 8811');