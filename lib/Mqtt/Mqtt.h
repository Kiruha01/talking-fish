#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "MotionController.h"
#include "HttpSpecFinder.h"

class Mqtt
{
private:
  PubSubClient psc;
  MotionController* motion;
    void reconnect();

  void runNewSpec(String newSpec){
    motion->setUpSpec(newSpec);
  }

public:

    Mqtt(WiFiClient& client, MotionController& controller);
    ~Mqtt();
    void tick();

    void setClient(WiFiClient client){
        psc.setClient(client);
    }
    

    void setup(const char* server){
      psc.setServer(server, 1883);
      psc.setCallback([this](char* topic, uint8_t* payload, unsigned int length) {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < length; i++) {
            Serial.print((char)payload[i]);
        }
        Serial.println();

        char* buf = new char[length + 1];
        for (size_t i = 0; i < length; ++i)
          buf[i] = (char)payload[i];
        
        buf[length] = '\0';
        if (String(topic) == "text/topic"){
          this->runNewSpec(String(buf));
        }
    });
    }
};
  


Mqtt::Mqtt(WiFiClient& client, MotionController& controller) {
    psc = PubSubClient(client);
    motion = &controller;
    
}

Mqtt::~Mqtt()
{
}

void setUpHS(PubSubClient& client) {
    String output;

    // client.setBufferSize(300);

    JsonDocument dev;
    dev["ids"] = "idididid";
    dev["name"] = "Fishing";
    dev["mf"] = "Kirill";
    dev["mdl"] = "model";
    dev["sw"] = "1.0";
    dev["sn"] = ESP.getChipId();

    JsonDocument o;
    o["name"] = "fish2text";
    o["sw"] = "1.0";

    JsonDocument doc;
    doc["dev"] = dev;
    doc["o"] = o;
    doc["command_topic"] = "text/topic";
    doc["name"] = "Text to say!";
    doc["unique_id"] = "fish_1";
    doc["qos"] = 0;

    serializeJson(doc, output);
    
    client.publish("homeassistant/text/talking_fish/config", output.c_str(), output.length());
    client.subscribe("text/topic");
}


void Mqtt::reconnect() {
    // Loop until we're reconnected
    while (!psc.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      Serial.println(clientId);
      // Attempt to connect
      if (psc.connect(clientId.c_str())) {
        Serial.println("connected");
        setUpHS(psc);
        Serial.println("configured");
      } else {
        Serial.print("failed, rc=");
        Serial.print(psc.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
      }
    }
  }
  

void Mqtt::tick() {
    if (!psc.connected()) {
      Serial.println("not connected");
        reconnect();
      }
      psc.loop();
  }

