#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR AP SSID";
const char* password = "YOUR AP PASSWORD";

// Config MQTT Server
#define mqtt_server "broker.hivemq.com"
#define mqtt_port 1883 

#define LED_PIN 2

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("rmutikkc/emc");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  
  // Logic การควบคุม
  if (msg == "GET") {
    client.publish("rmutikkc/emc", (digitalRead(LED_PIN) ? "LEDON" : "LEDOFF"));
    Serial.println("Sent Status!");
  } else if (msg == "LEDON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
  } else if (msg == "LEDOFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
  } else {
    Serial.print("Unknown command: ");
    Serial.println(msg);
  }
}