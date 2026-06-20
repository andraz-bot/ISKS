#include <Arduino.h>
void setup() {
  
  pinMode(15, OUTPUT); // nožica št. 15 bo delovala kot izhod
  pinMode(4, OUTPUT); // nožica št. 4 bo delovala kot izhod
}
void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja
  digitalWrite(15, HIGH); // na nožici 15 zapišemo visoko vrednost - LED zasveti
  digitalWrite(4, HIGH); // na nožici 4 zapišemo visoko vrednost - LED zasveti
  delay(250); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(15, LOW); // na nožici 15 zapišemo nizko vrednost - LED ugasne
  digitalWrite(4, LOW); // na nožici 4 zapišemo nizko vrednost - LED ugasne
  delay(250); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
}
