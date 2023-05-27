#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

// ========= CONFIG =========
#include "config.h"
#define DEBUG_FLAG   

#ifndef STASSID
#define STASSID "ssid"
#define STAPSK  "password"
#endif

//#include "MusicPlayer.h"
#include "MotionController.h"

// ==========================

// ----------- AudioRelated -------------------

const char* ssid = STASSID;
const char* password = STAPSK;

//MusicPlayer player;
MotionController motion;

/* ################################## Setup ############################################# */
String URL="http://192.168.0.101:8080/data";

void setup() {
  #ifdef DEBUG_FLAG
  Serial.begin(115200);
#endif
  delay(1000);
    #ifdef DEBUG_FLAG
  Serial.println("Connecting to WiFi");
    #endif
    
  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);

  // Try forever
  while (WiFi.status() != WL_CONNECTED) {
      #ifdef DEBUG_FLAG
    Serial.println("...Connecting to WiFi");
      #endif

    delay(1000);
  }
      #ifdef DEBUG_FLAG
  Serial.println("Connected");
      #endif


//  player.play(URL);
  motion.setSmt(getJsonData(URL + ".json"));
}

void loop() {
//   player.loopTick();
}

String getJsonData(String url){
  WiFiClient client;

    HTTPClient http;
    if (http.begin(client, url)) {
      int httpCode = http.GET();
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          return payload;
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
      } else {
        Serial.printf("[HTTP} Unable to connect\n");
      }
      return "";
    
}
