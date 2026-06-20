#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "WebSocketsServer.h"

const char* ssid = "Liberty";
const char* password = "Maun2003";

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

uint8_t LED1pin = 2;
bool LED1status = LOW;

String HTML0 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
<meta charset='UTF-8'>\n\
<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
<title>ESP32</title>\n\
</head>\n\
<body>\n\
<h2>ESP32 WebSocket strežnik</h2>\n\
<p>WebSocket deluje na portu 81</p>\n\
</body>\n\
</html>\n";

void handle_root()
{
  server.send(200, "text/html; charset=utf-8", HTML0);
}

void handle_NotFound()
{
  server.send(404, "text/html", "Spletna stran ni bila najdena");
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{

  switch(type)
  {

    case WStype_DISCONNECTED:
      Serial.printf("[%u] Klient odklopljen\n", num);
    break;

    case WStype_CONNECTED:
    {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Klient povezan iz IP naslova: ", num);
      Serial.println(ip.toString());
    }
    break;

    case WStype_TEXT:
    {
      Serial.printf("[%u] Prejeto sporočilo: %s\n", num, payload);

      if(strcmp((char*)payload, "LED_ON") == 0)
      {
        digitalWrite(LED1pin, HIGH);
      }

      if(strcmp((char*)payload, "LED_OFF") == 0)
      {
        digitalWrite(LED1pin, LOW);
      }
    }
    break;

    default:
    break;
  }
}

void setup()
{

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("IP naslov moudla esp32 je: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);
  server.onNotFound(handle_NotFound);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

  digitalWrite(LED1pin, HIGH);
  delay(500);
  digitalWrite(LED1pin, LOW);
}

void loop()
{
  server.handleClient();
  webSocket.loop();
}