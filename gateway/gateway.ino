#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>

const char* ssid = "UB_Connect";
const char* password = "";
const char* mqtt_server = "mqtt.eclipse.org";
int mqtt_port = 1883;
const char* dataTopic = "mesh_gateway/data";

WiFiClientSecure espClient;
PubSubClient mqtt_client(espClient);
char data[128];

// Connect to MQTT broker
void mqtt_connect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    log("Connecting to MQTT...");
    // Attempt to connect
    String mqtt_clientId = "mesh_gateway-";
    mqtt_clientId += String(random(0xffff), HEX);
    if (mqtt_client.connect(mqtt_clientId.c_str(), mqtt_username, mqtt_password)) {
      log("connected");
    } else {
      log("failed, rc=", false);
      log(mqtt_client.state());
      delay(2000);
    }
  }
}

void setup()   {
  pinMode(2, OUTPUT); // ESP8266 LED
  digitalWrite(2, LOW);
  delay(200);
  digitalWrite(2, HIGH);

  Serial.begin(9600);
  while (!Serial);

  log("Connecting to ");
  log(ssid, false);
  log("...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
//    log(".", false);
  }
//  log("");
//  log("connected");

  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_connect();
}

void log(const char *s, boolean newline) {
  if (newline) {
    return log(s);
  }
}

void loop() {
  if (!mqtt_client.connected()) {
    mqtt_connect();
  }
  mqtt_client.loop();

  delay(50); // Give the ESP time to handle network.

  if (Serial.available()) {
    Serial.setTimeout(100);
    String s = Serial.readStringUntil('\n');
    Serial.println(s);
    char * data = s.c_str;
    mqtt_client.publish(dataTopic, data);

  }
}
