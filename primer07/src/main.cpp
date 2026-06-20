#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"
#include <WiFiAP.h>


const char* ssid = "esp32_admin";
const char* password = "12345678";

// port 80 je prednastavljen
WebServer server(80);


// vsebina html strani ki se bo posredovala iz našega esp32 spletnega strežnika na brskalnik klienta, tu lahko dodamo tudi css
//naša spletna stran bo spramvljena v spremenljivki html tipa string

String HTML = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>esp32</title>\n\
</head>\n\
<body>\n\
    <h1>Pozdravljen svet iz esp32! čšž-ji delujejo!</h1>\n\
</body>\n\
</html>";

// tu določimo kaj se dogodi, če uporabnik  vpiše
// korenski ("root" naslov našega strežnika na esp32 modul 
//napr. http://192.168.88.98)
void handle_root(){
  server.send(200, "text/html; charset=utf-8", HTML);
}

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se izvede le enkrat - nastavitve

  Serial.begin(115200); 


  //vzpostavitev programske dostopne točke - "Soft Access Point" AP
  //software enabled access point - npr. računalnik, ki ni bil zasnovan kot dostopna točka postane z ustreznim programom dostopna točka
  WiFi.softAP(ssid,password);


//izpišemo IP naslov
    Serial.println("Vzpostavitev programske dostopne točke na esp32 modulu je uspela!");
    Serial.print("IP naslov esp32 dostopne točke je: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", handle_root); // ko vpišemo v brskalnik http://192.168.88.98/ + enter strežnik to zazna kot "request" in na ta "reqest" (zahtevo)n sproži funkcijo handle_root, ki posreduje "resoinse (odgovor) oz. server. send (200; "text/html"; "Pozdrav");;"
    server.begin(); // zažene strežnik

    Serial.print("HTTP strežnik je zagnan, vpišite IP naslov v brskalnik: ");
    Serial.println(WiFi.softAPIP());


    // del, ki služi kot indikator, da je koda prenesena na modul
    pinMode(2, OUTPUT);          // nožica št. 2 bo delovala kot izhod
    digitalWrite(2, HIGH);       // LED zasveti
    delay(500);                  // zakasnitev 500 ms
    digitalWrite(2, LOW);        // LED ugasne
    delay(500);                  // zakasnitev 500 ms
    digitalWrite(2, HIGH);       // LED zasveti
    delay(500);                  // zakasnitev 500 ms
    digitalWrite(2, LOW);        // LED ugasne
}

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja
  server.handleClient(); // sprejema prisotnost klientov in posluša njihove html zahteve("request")
}