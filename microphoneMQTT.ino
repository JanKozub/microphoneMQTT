#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define mic A0
#define LED D2

char* ssid;
char* password;
char* mqtt_server;

WiFiClient espClient;
PubSubClient client(espClient);

int minValue = 500;
int configValue = 50;

int checkTime = 0;
int lastTime = 0;

boolean state = false;

void setup() {
  importPrivateData();

  pinMode(mic, INPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
  long now = millis();

  if (!client.connected()) {
    reconnect();
  } else {
    client.loop();

    if (now - lastTime > 5000) {
      lastTime = now;
      publishStatus();
      ESP.wdtFeed();
    }
  }

  if (analogRead(A0) > minValue + configValue) {
    checkTime = millis();
    delay(70);
    do {
      if (analogRead(A0) > minValue + configValue) {
        delay(70);
        state = !state;
        digitalWrite(LED, state);
        publishStatus();
        ESP.wdtFeed();
        break;
      }
    } while (millis() - checkTime < 1000);
  }
}

void publishStatus() {
  String msg = "microphone," + String(state) + "," + String(configValue);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("microphone", msg.c_str());
}
