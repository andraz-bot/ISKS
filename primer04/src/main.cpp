#include <Arduino.h>
#include "Wifi.h"
#include "WebServer.h"

const char* ssid = "BUM";
const char* password = "N0gavica";


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
    <h1>Pozdravlen svet iz 1111, čšž</h1>\n\
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
    <h1>Pozdravlen svet iz esp32, čšž</h1>\n\
</body>\n\
</html>";


void handle_root(){ 
  server.send(200,"text/html", HTML3); 

}

void handle_1(){
  LED1status = HIGH;
  server.send(200,"text/html", HTML1);

}

void setup() {
  
  Serial.begin(115200);

  WiFi.begin(ssid,password); 

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Povezovanje z WiFi omrezjem...");
  }


  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("IP naslov esp32 modula je: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);
  server.on("/1", handle_1); 

  server.begin();
  

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
