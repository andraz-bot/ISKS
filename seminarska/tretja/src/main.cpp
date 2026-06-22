#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

// --- SEGMENT: Sistemska konfiguracija ---
const char* ssid = "Andraz’s iphone";
const char* password = "Marolt2005!";

// Definicije pinov za aktuatorje (LED) in senzorje (potenciometer, tipka)
const int ledPini[] = {27, 25, 26}; // Modra, Zelena, Rdeča
const int potPin = 35;
const int tipkaPin = 18;

WebSocketsClient webSocket;

void setup() {
    Serial.begin(115200);
    
    // Inicializacija pinov: LED diode kot izhodi, tipka kot vhod s pull-up uporom
    for(int i=0; i<3; i++) pinMode(ledPini[i], OUTPUT);
    pinMode(tipkaPin, INPUT_PULLUP);

    // Povezava na lokalno WiFi omrežje
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);

    // Vzpostavitev WebSocket odjemalca za realnočasovno telemetrijo
    webSocket.begin("172.20.10.3", 8080);
}

void loop() {
    // WebSocket "heartbeat": ključno za vzdrževanje stabilne povezave s strežnikom
    webSocket.loop(); 
    
    // --- SEGMENT 1: Detekcija dogodkov (Edge detection) ---
    // Zaznava pritiska tipke s preprečevanjem odbojev (debounce)
    static bool zadnjeStanje = HIGH;
    bool trenutnoStanje = digitalRead(tipkaPin);
    if (trenutnoStanje == LOW && zadnjeStanje == HIGH) {
        // Pošiljanje ukaza v JSON formatu (standardizirana komunikacija)
        webSocket.sendTXT("{\"ukaz\": \"klik\"}");
        delay(50); 
    }
    zadnjeStanje = trenutnoStanje;

    // --- SEGMENT 2: Lokalna kontrola (Hysteresis/Logic) ---
    // ESP32 izvaja logiko lokalno za zagotavljanje takojšnjega odziva (nizka latenca)
    int val = analogRead(potPin);
    digitalWrite(ledPini[0], val < 1948);                     // Modra: prenizka vrednost
    digitalWrite(ledPini[1], val >= 1948 && val <= 2148);     // Zelena: idealno območje
    digitalWrite(ledPini[2], val > 2148);                     // Rdeča: previsoka vrednost

    // --- SEGMENT 3: Asinhrono pošiljanje telemetrije ---
    // Uporaba časovnika (millis) preprečuje blokiranje zanke (npr. z delay())
    static unsigned long zadnjiCas = 0;
    if (millis() - zadnjiCas > 100) {
        // Serializacija podatkov v JSON za prenos v brskalnik
        StaticJsonDocument<200> doc;
        doc["val"] = val; 
        String output;
        serializeJson(doc, output); 
        
        webSocket.sendTXT(output); // Pošiljanje paketa na WebSocket strežnik
        zadnjiCas = millis();
    }
}