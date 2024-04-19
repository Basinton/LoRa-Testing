/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
// #include <string.h>

// Replace with your network credentials
const char *ssid = "BKIT_LUGIA_CS2";
const char *password = "cselabc5c6";

const char *ap_ssid = "LORA_TESTING";
const char *ap_password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Create an Event Source on /events
AsyncWebSocket ws("/ws");

String message = "";

int route = 50;
char *driverName = "Thịnh";
float lat = 10.772067412563548;
float lng = 106.65784201489897;
int speed = 30;
char *direction = "Đi đến Đại học Quốc gia";
char *nextStop = "Lăng Cha Cả";

// Json Variable to Hold Slider Values
JSONVar busInfo;

// Get Slider Values
String getBusInfo()
{
  busInfo["bus_route"] = String(route);
  busInfo["bus_driver_name"] = String(driverName);
  busInfo["bus_location_lat"] = String(lat, 14);
  busInfo["bus_location_lng"] = String(lng, 14);
  busInfo["bus_speed"] = String(speed);
  busInfo["bus_direction"] = String(direction);
  busInfo["bus_next_stop"] = String(nextStop);

  String jsonString = JSON.stringify(busInfo);
  return jsonString;
}

// Initialize SPIFFS
void initFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");
  }
}

// Initialize WiFi
void initWiFi()
{
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  // WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   Serial.print('.');
  //   delay(1000);
  // }
  // Serial.println(WiFi.localIP());
}

void notifyClients(String busInfo)
{
  ws.textAll(busInfo);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    message = (char *)data;
    Serial.println(message);
    if (strcmp((char *)data, "getBusInfo") == 0)
    {
      notifyClients(getBusInfo());
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

unsigned long current = 0;
unsigned long previous = 0;
int timerDelay = 5000;
int switchPlace = 0;

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  initFS();

  // Connect to Wi-Fi
  initWiFi();

  // initWebSocket();

  // // Route for root / web page
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send(SPIFFS, "/index.html", "text/html"); });

  // server.serveStatic("/", SPIFFS, "/");

  // server.begin();
}

void loop()
{
  // current = millis();
  // if (current - previous > timerDelay)
  // {
  //   previous = current;
  //   if (switchPlace == 0)
  //   {
  //     lat = 10.772067412563548;
  //     lng = 106.65784201489897;
  //     nextStop = "Trường Đại học Bách khoa";
  //     switchPlace = 1;
  //   }
  //   else if (switchPlace == 1)
  //   {
  //     lat = 10.767827618834696;
  //     lng = 106.6558693609582;
  //     nextStop = "Sân vận động Phú Thọ";
  //     switchPlace = 0;
  //   }
  //   ws.cleanupClients();
  // }
}