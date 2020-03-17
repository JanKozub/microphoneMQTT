void setup_wifi() {

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "Microphone";

    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      client.publish("microphone", "microphone,1,100");

      client.subscribe("microphoneSub");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String output = "";
  for (int i = 0; i < length; i++) {
    output = output + (char)payload[i];
  }
  if (output.equals("GET")) {
    String msg = "microphone," + String(state) + "," + String(configValue);
    publishStatus();
  }
//  if (output.indexOf("SET") > 0) {
//    String value = output.substring(4, 7);
//    configValue = value.toInt();
//    publishStatus();
//  }
}
