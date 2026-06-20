#include <Arduino.h>
#include "Wifi.h"
#include "WebServer.h"

const char* ssid = "BUM";
const char* password = "N0gavica";


WebServer server(80); 

uint8_t LED1pin = 2; //8-bitna celostevilska spremenljivka
uint8_t LED2pin = 4;
uint8_t LED3pin = 19;

bool LED1status = LOW;
bool LED2status = LOW;
bool LED3status = LOW;

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

String HTML4 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>Document</title>\n\
</head>\n\
<body>\n\
    <h1>Prejet ukaz za izklop zelene LED diode</h1>\n\
</body>\n\
</html>";

String HTML5 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>Document</title>\n\
</head>\n\
<body>\n\
    <h1>Prejet ukaz za vklop zelene LED diode</h1>\n\
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

void handle_3(){
  LED2status = HIGH;
  Serial.println("GPIO2 Status: on");
  server.send(200,"text/html", HTML3);

}

void handle_2(){
  LED2status = LOW;
  Serial.println("GPIO2 Status: off");
  server.send(200,"text/html", HTML2);

}

void handle_5(){
  LED3status = HIGH;
  Serial.println("GPIO2 Status: on");
  server.send(200,"text/html", HTML5);

}

void handle_4(){
  LED3status = LOW;
  Serial.println("GPIO2 Status: off");
  server.send(200,"text/html", HTML4);

}

void handle_NotFound(){ //ce ne najdemo spletne strani
   server.send(404,"text/html", "Spletna stran ni bila najdena");

}

void setup() {

  pinMode(LED1pin,OUTPUT);
   pinMode(LED2pin,OUTPUT);
    pinMode(LED3pin,OUTPUT);

  
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
  server.on("/0", handle_0); 
  server.on("/1", handle_1); 
  server.on("/2", handle_2); 
  server.on("/3", handle_3); 
   server.on("/4", handle_4); 
    server.on("/5", handle_5); 
  
  server.onNotFound(handle_NotFound); 

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

  if(LED2status){
    digitalWrite(LED2pin, HIGH);
  }
  else{
    digitalWrite(LED2pin,LOW);
  }

  if(LED3status){
    digitalWrite(LED3pin, HIGH);
  }
  else{
    digitalWrite(LED3pin,LOW);
  }
}
