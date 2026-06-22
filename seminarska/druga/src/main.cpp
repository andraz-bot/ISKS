#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "Andraz’s iphone";
const char* password = "Marolt2005!";

// Definiramo pine za LED-ice in ustrezne touch senzorje
const int ledPini[] = {27, 25, 26}; // Modra, Zelena, Rdeča
const int touchPini[] = {T0, T3, T4}; // Odgovarjajoči touch pini na ESP32
const int tipkaPin = 18;

WebSocketsClient webSocket;

void setup() {
    Serial.begin(115200);
    for(int i=0; i<3; i++) pinMode(ledPini[i], OUTPUT);
    pinMode(tipkaPin, INPUT_PULLUP);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);

    // Vzpostavitev WebSocket klienta do Node.js strežnika
    webSocket.begin("172.20.10.3", 8080);
    webSocket.setReconnectInterval(5000); // Avtomatsko ponovno povezovanje ob izpadu
}

void loop() {
    webSocket.loop(); // Vzdrževanje WebSocket povezave
    
    // --- 1. Logika za tipko ---
    // Pošlje komando za dinamično dodajanje/brisanje grafov v brskalniku
    static bool zadnjeStanjeTipke = HIGH;
    bool trenutnoStanje = digitalRead(tipkaPin);
    
    if (trenutnoStanje == LOW && zadnjeStanjeTipke == HIGH) {
        webSocket.sendTXT("{\"ukaz\": \"klik\"}"); 
        delay(50); 
    }
    zadnjeStanjeTipke = trenutnoStanje;

    // --- 2. Logika za touch in LED ---
    // Vsakih 100ms preverimo vse 3 senzorje
    static unsigned long zadnjiCas = 0;
    if (millis() - zadnjiCas > 100) { 
        for(int i = 0; i < 3; i++) {
            int touchVrednost = touchRead(touchPini[i]); // Branje kapacitivnosti

            // Lokalna inteligenca: če je vrednost majhna (dotik), preklopimo LED
            if(touchVrednost < 30) {
                digitalWrite(ledPini[i], !digitalRead(ledPini[i]));
            }
            
            // Serializacija v JSON za pošiljanje preko WebSocketa
            // Vsak senzor ima svoj 'id', da brskalnik ve, kateri graf posodobiti
            StaticJsonDocument<200> doc;
            doc["id"] = i;
            doc["val"] = touchVrednost;
            String output;
            serializeJson(doc, output);
            webSocket.sendTXT(output);
        }
        zadnjiCas = millis();
    }
}