#include <Arduino.h>
#include "Wifi.h"
#include "WebServer.h"
#include <WiFiAP.h>

const char* ssid = "esp_32andraz";
const char* password = "12345678";


WebServer server(80); 

uint8_t LED1pin = 2; //8-bitna celostevilska spremenljivka


bool LED1status = LOW;


//vsebina HTML strani ki se bo posredovala iz nasega esp32 na brskalnik klienta,lahko dodamo tudi CSS
//nasa spletna stran bo shranjena v spremenljivki HTML tipa string

String HTML1 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>Document</title>\n\
</head>\n\
<body>\n\
    <h1>Prejet ukaz za vklop modre LED diode</h1>\n\
</body>\n\
</html>";

String HTML2 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>Document</title>\n\
</head>\n\
<body>\n\
    <h1>Prejet ukaz za izklop rdece LED diode</h1>\n\
</body>\n\
</html>";

String HTML0 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>Document</title>\n\
</head>\n\
<body>\n\
    <h1>Prejet ukaz za izklop modre LED diode</h1>\n\
</body>\n\
</html>";

String HTML3 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>Document</title>\n\
</head>\n\
<body>\n\
    <h1>Prejet ukaz za vklop rdece LED diode</h1>\n\
</body>\n\
</html>";




void handle_root(){ 
  server.send(200,"text/html", HTML3); 
}

void handle_1(){
  LED1status = HIGH;
  Serial.println("GPIO2 Status: ON");
  server.send(200,"text/html", HTML1);

}

void handle_0(){
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200,"text/html", HTML0);

}


void handle_NotFound(){ //ce ne najdemo spletne strani
   server.send(404,"text/html", "Spletna stran ni bila najdena");

}

void setup() {

  Serial.begin(115200);

   WiFi.softAP(ssid,password);


//izpišemo IP naslov
    Serial.println("Vzpostavitev programske dostopne točke na esp32 modulu je uspela!");
    Serial.print("IP naslov esp32 dostopne točke je: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", handle_root);// ko vpišemo v brskalnik http://192.168.88.98/ + enter strežnik to zazna kot "request" in na ta "reqest" (zahtevo)n sproži funkcijo handle_root, ki posreduje "resoinse (odgovor) oz. server. send (200; "text/html"; "Pozdrav");;"
    server.on("/0", handle_0); 
    server.on("/1", handle_1);
    server.onNotFound(handle_NotFound); 
    server.begin();

    Serial.print("HTTP strežnik je zagnan, vpišite IP naslov v brskalnik: ");
    Serial.println(WiFi.softAPIP());
  

  pinMode(2, OUTPUT); 
  digitalWrite(2, HIGH); 
  digitalWrite(2, LOW); 
  digitalWrite(2, HIGH); 
  delay(500); 
  digitalWrite(2, LOW); 

}


void loop() {
  server.handleClient(); 

  if(LED1status){
    digitalWrite(LED1pin, HIGH);
  }
  else{
    digitalWrite(LED1pin,LOW);
  }
}
