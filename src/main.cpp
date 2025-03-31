#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

// ========= CONFIG =========
#define DEBUG_FLAG

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PASS"
#endif

#include "MotionController.h"

// ==========================

const char* ssid = STASSID;
const char* password = STAPSK;

MotionController motion;

/* ################################## Setup ############################################# */
String URL = "http://192.168.0.105/files/fish-sing";


void setup() {
  motion.setUp();
  Serial.begin(115200);
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

  motion.specFinder.setBaseUrl("http://192.168.0.105/files");
  motion.setUpSpec("fish-sing");
}

/* #################################### Loop ############################################## */

void loop() {
  motion.tick();
}

/* ################################## Network ############################################# */

// #include "Arduino.h"
// #include <ArduinoJson.h>


// const String jsonString = "{\"one\": [1,2,3,222,456754323,1231232323232323232, \"sadfad\"]}";

// void setup(){
//   Serial.begin(115200);
// }

// void loop(){
//   JsonDocument* jsonDoc = new JsonDocument(); // TODO: on json size
//   DeserializationError err = deserializeJson(*jsonDoc, jsonString);
//   if (err != DeserializationError::Ok){
//     Serial.println("Error");
//     Serial.println(err.f_str());
//   }
//   if ((*jsonDoc)["one"].is<JsonArray>()) {
//     Serial.println("is json array");
//       JsonArray s = (*jsonDoc)["one"];
//       int headLen = s.size();
//       Serial.println(headLen);
//       for (int i = 0; i<headLen; ++i){
//         Serial.print(s[i].as<String>());
//         Serial.print(" (");
//         Serial.print(s[i].as<long long>());
//         Serial.print(") ");
//         Serial.print(s[i].is<int>());
//         Serial.print(" ");
//       }
//     }
//   delete jsonDoc;
//   delay(200);
// }