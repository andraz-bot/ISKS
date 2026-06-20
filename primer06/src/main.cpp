#include <Arduino.h>
#include "Wifi.h"
#include "WebServer.h"

const char* ssid = "BUM";
const char* password = "N0gavica";


WebServer server(80); 

uint8_t LED1pin = 4;

bool LED1status = LOW;

//vsebina HTML strani ki se bo posredovala iz nasega esp32 na brskalnik klienta,lahko dodamo tudi CSS
//nasa spletna stran bo shranjena v spremenljivki HTML tipa string

const char HTML1[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>

    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <style> 
        html {font-family: Helvetica; text-align: center;}
        body {margin-top: 50px;}
        h1 {color: #444444; margin: 50px auto 30px;}
        h3 {color: #444444; margin-bottom: 50px;}
        p {font-size: 14px; color: #222222; margin-bottom: 10px;}
        .gumb {background-color: #3498db; display: block; width: 200px; color: white; padding: 13px 30px; text-decoration: none; font-size: 25px; margin: 0px auto 35px; cursor: pointer; border-radius: 4px; border: none;}
    </style>

    <title>esp32</title>

</head>

<body>

    <h1>ESP32 spletni strežnik</h1>
    <h3>Priklop na WiFi usmerjevalnik</h3>

    <a class='gumb' href="/1">Prižgi LED</a>
    <a class='gumb' href="/0">Ugasni LED</a>
    <p>Prejet ukaz za vklop LED diode..</p>

</body>
</html> 
)rawliteral";



const char HTML0[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>

    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <style> 
        html {font-family: Helvetica; text-align: center;}
        body {margin-top: 50px;}
        h1 {color: #444444; margin: 50px auto 30px;}
        h3 {color: #444444; margin-bottom: 50px;}
        p {font-size: 14px; color: #222222; margin-bottom: 10px;}
        .gumb {background-color: #3498db; display: block; width: 200px; color: white; padding: 13px 30px; text-decoration: none; font-size: 25px; margin: 0px auto 35px; cursor: pointer; border-radius: 4px; border: none;}
    </style>

    <title>esp32</title>

</head>

<body>

    <h1>ESP32 spletni strežnik</h1>
    <h3>Priklop na WiFi usmerjevalnik</h3>

    <a class='gumb' href="/1">Prižgi LED</a>
    <a class='gumb' href="/0">Ugasni LED</a>
    <p>Prejet ukaz za izklop LED diode</p>

</body>
</html> 
)rawliteral";


String HTML3 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <style>\n\
        html {font-family: Helvetica; text-align: center;}\n\
        body {margin-top: 50px;}\n\
        h1 {color: #444444; margin: 50px auto 30px;}\n\
        h3 {color: #444444; margin-bottom: 50px;}\n\
        p {font-size: 14px; color: #222222; margin-bottom: 10px;}\n\
        .gumb {background-color: #3498db; display: block; width: 200px; color: white; padding: 13px 30px; text-decoration: none; font-size: 25px; margin: 0px auto 35px; cursor: pointer; border-radius: 4px; border: none;}\n\
    </style>\n\
    <title>esp32</title>\n\
</head>\n\
<body>\n\
    <h1>ESP32 spletni strežnik</h1>\n\
    <h3>Priklop na WiFi usmerjevalnik</h3>\n\
    <a class='gumb' href='/1'>Prižgi LED</a>\n\
    <a class='gumb' href='/0'>Ugasni LED</a>\n\
    <p>HTTP strežnik je zagnan.</p>\n\
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

  pinMode(LED1pin,OUTPUT);


  
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
}
