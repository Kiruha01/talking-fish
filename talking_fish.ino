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

#ifndef HEAD_PIN
#define HEAD_PIN D6
#define TAIL_PIN D7
#define MOUTH_PIN D8
#endif

#include "MusicPlayer.h"
#include "MotionController.h"

// ==========================

const char* ssid = STASSID;
const char* password = STAPSK;

MusicPlayer player;
MotionController motion;

/* ################################## Setup ############################################# */
String URL = "http://192.168.0.105/files/fish-sing";

void setup() {
  pinMode(HEAD_PIN, OUTPUT);
  pinMode(TAIL_PIN, OUTPUT);
  pinMode(MOUTH_PIN, OUTPUT);
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
  player.play(URL + ".mp3");
  motion.setMotion(getJsonData(URL + ".json"));
  motion.start();


}

/* #################################### Loop ############################################## */

void loop() {
  player.loopTick();
  motion.loopTick();
}

/* ################################## Network ############################################# */

String getJsonData(String url) {
  WiFiClient client;

  HTTPClient http;
  if (http.begin(client, url)) {
    int httpCode = http.GET();
    if (httpCode > 0) {
#ifdef DEBUG_FLAG
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
#endif


      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
#ifdef DEBUG_FLAG
        Serial.println(payload);
#endif
        http.end();
        return payload;
      }
    } else {
      Serial.println(url);
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
  return "";

}
