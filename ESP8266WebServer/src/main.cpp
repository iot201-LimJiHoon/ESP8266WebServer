#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "limjihoonzzang";
const char* password = "dlawlgns99";

ESP8266WebServer server(80);

void handleRoot() {
  String message = (server.method() == HTTP_GET)?"GET":"POST";
  message += " " + server.uri() + "\n";
  for (uint8_t i=0; i < server.args(); i++) {
    message += " " + server.argName(i) + " : " + server.arg(i) + "\n";
  }
  message += "\nHello from ESP8266!\n";
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connextion
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());

  if (MDNS.begin("miniWeb")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  // This is an example of inline function for a handler
  server.on("/inline", []() {
    server.send(200, "text/plain", "Hello from the inline function\n");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  MDNS.update();
  server.handleClient();
}