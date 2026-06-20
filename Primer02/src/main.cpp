#include <Arduino.h>
#include "Wifi.h"
#include "WebServer.h"

const char* ssid = "BUM";
const char* password = "N0gavica";


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
}
