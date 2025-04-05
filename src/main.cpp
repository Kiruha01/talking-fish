// #define MQTT_MAX_PACKET_SIZE 300
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "Mqtt.h"
#include "MotionController.h"
// Update these with values suitable for your network.

const char* ssid = STASSID;
const char* password = STAPSK;
const char* mqtt_server = "192.168.0.105";

WiFiClient espClient;
// PubSubClient psc = PubSubClient(espClient);
MotionController motion;
Mqtt mqtt(espClient, motion);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);
  // Serial.begin(115200);
  setup_wifi();
  mqtt.setup(mqtt_server);
  motion.setUp();
  motion.specFinder.setBaseUrl("http://192.168.0.105/files");
  digitalWrite(BUILTIN_LED, LOW);
  // motion.setUpSpec("fish-sing");

}

void loop() {
  // Serial.println("loop");
  motion.tick();
  mqtt.tick();
}