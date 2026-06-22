#include <Arduino.h>
#include "WiFi.h"
#include "WebSocketsClient.h"
#include <ArduinoJson.h>

const char* ssid = "Andraz’s iphone";
const char* password = "Marolt2005!";


// Definicija pinov za senzor in LED-ice
const int nozicaFotoupornika = 32;
const int ledGlavna = 25; 
const int ledRezerva = 27;
const int ledModra = 2; // Vgrajena LED za diagnostiko

WebSocketsClient webSocket;
bool vklopLED = false;       // Željeno stanje (iz spletne strani)
unsigned long zadnjiUtrip = 0;
bool modraStanje = false;

// Funkcija za obdelavo dohodnih ukazov iz strežnika
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);
    // Preverimo, če je ukaz za LED
    if (doc.containsKey("tipSporočila") && doc["tipSporočila"] == "LED") {
      vklopLED = doc["vrednost"];
      Serial.printf("Prejet ukaz: %d\n", vklopLED);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledGlavna, OUTPUT);
  pinMode(ledRezerva, OUTPUT);
  pinMode(ledModra, OUTPUT);
  
  WiFi.begin(ssid, password);
  // Povezava na WebSocket strežnik (port 8811 - ločen kanal za ESP32)
  webSocket.begin("172.20.10.5", 8811);
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop(); // Vzdrževanje povezave
  int vrednost = analogRead(nozicaFotoupornika);

  // LOGIKA POVRATNE ZANKE (Feedback Loop & Diagnostic)
  if (vklopLED) {
    // Če fotoupor zazna nizko vrednost (temo), čeprav bi morala LED svetiti
    if (vrednost < 700) { 
      // DIAGNOSTIKA: Glavna luč ne deluje (napaka)
      digitalWrite(ledGlavna, LOW);
      digitalWrite(ledRezerva, HIGH); // Avtomatski preklop na rezervo
      
      // Utripajoča modra LED kot vizualni alarm
      if (millis() - zadnjiUtrip > 300) {
        modraStanje = !modraStanje;
        digitalWrite(ledModra, modraStanje);
        zadnjiUtrip = millis();
      }
    } else {
      // VSE OK: Sistem deluje pravilno
      digitalWrite(ledGlavna, HIGH);
      digitalWrite(ledRezerva, LOW);
      digitalWrite(ledModra, LOW);
      modraStanje = false;
    }
  } else {
    // SISTEM IZKLOPLJEN: Vse ugasnjeno
    digitalWrite(ledGlavna, LOW);
    digitalWrite(ledRezerva, LOW);
    digitalWrite(ledModra, LOW);
  }

  // Pošiljanje telemetrije v brskalnik (vsakih 200ms)
  static unsigned long zadnjePosiljanje = 0;
  if (millis() - zadnjePosiljanje > 200) {
    String dataString = "{\"tipSporočila\":\"fotoupornik\",\"vrednost\":" + String(vrednost) + "}";
    webSocket.sendTXT(dataString);
    zadnjePosiljanje = millis();
  }
}