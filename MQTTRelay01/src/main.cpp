#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
#define             RELAY 15
const char*         ssid = "IoT518";
const char*         password = "iot123456";
const char*         mqttServer = "54.82.121.229";
const int           mqttPort = 1883;

unsigned long       interval = 10000;
unsigned long       lastPublished = - interval;
 
WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length);
void pubStatus();

void setup() {
    Serial.begin(115200);
    pinMode(RELAY, OUTPUT);
    WiFi.mode(WIFI_STA); 
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to the WiFi network");
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
 
    while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
 
        if (client.connect("ESP8266Hoon")) {
            Serial.println("connected");  
        } else {
            Serial.print("failed with state "); Serial.println(client.state());
            delay(2000);
        }
    }
    client.subscribe("id/jihoon/relay/cmd");
    digitalWrite(RELAY, LOW);
}

void loop() {
    client.loop();

    unsigned long currentMillis = millis();
    if(currentMillis - lastPublished >= interval) {
        lastPublished = currentMillis;
        pubStatus();
    }
}

void pubStatus() {
    char buf[10];
    if (digitalRead(RELAY) == HIGH) {
        sprintf(buf, "on");
    } else {
        sprintf(buf, "off");
    }
    client.publish("id/jihoon/relay/evt", buf);
}

void callback(char* topic, byte* payload, unsigned int length) {
 
    char msgBuffer[20];
    if(!strcmp(topic, "id/jihoon/relay/cmd")) {
        int i;
        for(i = 0; i < (int)length; i++) {
            msgBuffer[i] = payload[i];
        } 
        msgBuffer[i] = '\0';
        Serial.printf("\n%s -> %s", topic, msgBuffer);
        if(!strcmp(msgBuffer, "on")) {
            digitalWrite(RELAY, HIGH);
        } else if(!strcmp(msgBuffer, "off")) {
            digitalWrite(RELAY, LOW);
        }
    }
    pubStatus();
}