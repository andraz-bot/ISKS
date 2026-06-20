#include <Arduino.h>
#include "Wifi.h"
#include "WebServer.h"

const char* ssid = "BUM";
const char* password = "N0gavica";

// port 80 je prednastavljeni port za posredovanje html strani
WebServer server(80); //objekt razreda WebServer

//tu dolocimo kaj se zgodi ce uporabnik vpise korenski root naslov nasega streznika na esp modulu
//npr. http://172.30.0.94/

void handle_root(){ //ce dobimo rahtevo na streznik ("request")
  server.send(200, "text/html", "Pozdravljen svet iz esp32!"); //streznik poslje klientu kot odziv "response"
                                                               //String "Pozdravljen svet iz..."
                                                               //koda 200 predstavlja OK

}


void setup() {
  
  Serial.begin(115200); //hitrost komunikacije preko serijskega protokola(bitov/sekundo)

  WiFi.begin(ssid,password); //pozenemo wifi omrezje - se priklopimo

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Povezovanje z WiFi omrezjem...");
  }


  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("IP naslov esp32 modula je: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root); //ko vpisemo v brskalnik http://172.10.0.94/ +enter streznik to zazna kot request
  //"request" in na ta request sprozi funkcijo handle_root ki posreduje response

  server.begin();
  

  pinMode(2, OUTPUT); // nožica št. 2 bo delovala kot izhod
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne

}


void loop() {
  server.handleClient(); //spremlja prisotnost klientov in posluša njihove HTML zahteve ("request")
}
