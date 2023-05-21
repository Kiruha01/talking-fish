#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "AudioFileSourceHTTPStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"  // #include "AudioOutputI2S.h"
#define DEBUG_FLAG  

#ifndef STASSID
#define STASSID "ScoreBoard"
#define STAPSK  "12345678"
#endif

AudioGeneratorMP3         *mp3 = NULL;
AudioFileSourceHTTPStream *file_http = NULL;
AudioFileSourceBuffer     *buff = NULL;
AudioOutputI2SNoDAC       *out = NULL;

// AudioRelated ---------------------------
float volume_level              = 0.8;
String playing_status;
const int preallocateBufferSize = 2048;
void *preallocateBuffer = NULL;

const char* ssid = STASSID;
const char* password = STAPSK;

/* ################################## Setup ############################################# */
const char *URL="http://10.1.31.16:8080/splin.mp3";

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


#ifdef USE_I2S
  out = new AudioOutputI2S();
  Serial.println("Using I2S output");
#else
  out = new AudioOutputI2SNoDAC();
  Serial.println("Using No DAC - using Serial port Rx pin");
#endif
  out->SetGain(volume_level);

  file_http = new AudioFileSourceHTTPStream();
  if ( file_http->open(URL)) {
        buff = new AudioFileSourceBuffer(file_http, preallocateBuffer, preallocateBufferSize);
        mp3 = new AudioGeneratorMP3();
        mp3->begin(buff, out);
      } else {
        stopPlaying();
        Serial.println("error on open file");
      }

  
}

void loop() {
  if (mp3   && !mp3->loop())    stopPlaying();
#ifdef DEBUG_FLAG
  if (mp3 && mp3->isRunning()) {
    static int unsigned long lastms = 0;
    if (millis() - lastms > 1000) {
      lastms = millis();
      Serial.print(F("Free: "));
      Serial.print(ESP.getFreeHeap(), DEC);
      Serial.print(F("  ("));
      Serial.print( ESP.getFreeHeap() - ESP.getMaxFreeBlockSize(), DEC);
      Serial.print(F(" lost)"));
      Serial.print(F("  Fragmentation: "));
      Serial.print(ESP.getHeapFragmentation(), DEC);
      Serial.print(F("%"));
      if ( ESP.getHeapFragmentation() > 40) Serial.print(F("  ----------- "));
      Serial.println();
    }
  }
#endif
}

void stopPlaying() {

  if (mp3) {
#ifdef DEBUG_FLAG
    Serial.print(F("...#"));
    Serial.println(F("Interrupted!"));
    Serial.println();
#endif
    mp3->stop();
    delete mp3;
    mp3 = NULL;
  }
  if (buff) {
    buff->close();
    delete buff;
    buff = NULL;
  }
  if (file_http) {
    file_http->close();
    delete file_http;
    file_http = NULL;
  }
}
