import http from 'http';
import fs from 'fs';
import { WebSocketServer, WebSocket } from 'ws';

// 1. HTTP strežnik: skrbi za distribucijo spletnega vmesnika (HTML)
// Ko brskalnik vpiše IP naslov strežnika, mu pošljemo našo HTML datoteko
const server = http.createServer((req, res) => {
    fs.readFile('potenciometer.html', (err, data) => {
        if (err) {
            res.writeHead(404); // Napaka, če datoteke ni
            res.end("Datoteka ni najdena!");
        } else {
            res.writeHead(200, {'Content-Type': 'text/html'}); // Povemo brskalniku, da pošiljamo HTML
            res.end(data);
        }
    });
});

// Strežnik posluša na portu 8080 za vse mrežne vmesnike
server.listen(8080, '0.0.0.0');

// 2. WebSocket strežnik: skrbi za dvosmerno komunikacijo v realnem času
// Uporabljamo isti HTTP strežnik ("server"), da WebSocketi tečejo preko istih vrat
const wss = new WebSocketServer({ server });

// Dogodek, ko se katerakoli naprava (ESP32 ali brskalnik) poveže na strežnik
wss.on('connection', (ws) => {
    console.log("Nova naprava povezana!");
    
    // Ko prejmemo sporočilo od ene naprave, ga posredujemo naprej ostalim
    ws.on('message', (msg) => {
        // Broadcast (razpošiljanje): prehodimo seznam vseh povezanih odjemalcev
        wss.clients.forEach(client => {
            // Pošljemo samo tistim, ki imajo odprto povezavo (WebSocket.OPEN)
            if (client.readyState === WebSocket.OPEN) {
                client.send(msg.toString());
            }
        });
    });
});