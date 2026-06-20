#include <Arduino.h>

const int nozicaPotenciometra = 34;

int vrednostPotenciometra;


// konec funkcije webSocketEvent

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se bo izvedla le enkrat - nastavitve

  Serial.begin(115200); // hitrost komunikacije preko serijskega protokola

  Serial.println("Testiranje delovanja potenciometra na nožici 34");

  // del, ki služi kot indikator, de je koda prenešena na modul
  pinMode(2, OUTPUT); // nožica št. 2 bo delovala kot izhod
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasn

}

void loop() {

  vrednostPotenciometra = analogRead(nozicaPotenciometra);
  Serial.println(vrednostPotenciometra);
  delay(100);
  


}

