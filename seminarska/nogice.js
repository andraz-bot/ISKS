import { createServer } from 'http';
import { WebSocketServer } from 'ws';
import fs from 'fs';

// 1. HTTP STREŽNIK: Servira spletno stran (nogice.html)
// Uporabljamo ga za začetno nalaganje vmesnika v brskalniku
const server = createServer((req, res) => {
    fs.readFile('nogice.html', (err, data) => {
        if (err) {
            res.writeHead(404); // Če datoteka ne obstaja, pošljemo kodo 404
            res.end("Datoteka nogice.html ni najdena!");
            return;
        }
        res.writeHead(200, {'Content-Type': 'text/html'}); // Povemo brskalniku, da je vsebina HTML
        res.write(data);
        res.end();
    });
});

// 2. WEBSOCKET STREŽNIK: Ustvari kanal za realnočasovno komunikacijo
// WebSocket "živi" na istem HTTP strežniku, kar pomeni, da uporabljamo isti port (8080)
const wss = new WebSocketServer({ server });

// Ko se katera koli naprava (ESP32 ali brskalnik) poveže, se sproži ta dogodek
wss.on('connection', (ws) => {
    console.log('Nova naprava povezana!');
    
    // Ob prejemu katerega koli sporočila...
    ws.on('message', (message) => {
        const msgString = message.toString();
        console.log('Prejeto sporočilo:', msgString);

        // LOGIKA BROADCAST (Razpošiljanje):
        // Strežnik deluje kot "most". Ne zanima ga, KDO je poslal sporočilo,
        // ampak ga zanko posreduje VSEM ostalim povezanim napravam.
        wss.clients.forEach((client) => {
            // Preverimo, če je povezava z odjemalcem še aktivna (1 = OPEN)
            if (client.readyState === 1) { 
                client.send(msgString);
            }
        });
    });
});

// 3. ZAGON: Strežnik posluša na vseh vmesnikih (0.0.0.0) na portu 8080
server.listen(8080, '0.0.0.0', () => {
    console.log('Strežnik teče na http://172.20.10.3:8080');
});