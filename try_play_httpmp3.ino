#include "Arduino.h"
#include <ESP8266WiFi.h>

// ========= CONFIG =========
#include "config.h"
#define DEBUG_FLAG   

#ifndef STASSID
#define STASSID "ssid"
#define STAPSK  "password"
#endif

#include "MusicPlayer.h"

// ==========================

// ----------- AudioRelated -------------------

const char* ssid = STASSID;
const char* password = STAPSK;

MusicPlayer player;

/* ################################## Setup ############################################# */
const char *URL="http://192.168.0.101:8080/splin.mp3";

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


  player.play(URL);
}

void loop() {
   player.loopTick();
}
