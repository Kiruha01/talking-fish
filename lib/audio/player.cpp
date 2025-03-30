#include "player.h"

MusicPlayer::MusicPlayer() {
    #ifdef USE_I2S
    out = new AudioOutputI2S();
            Serial.println("Using I2S output");
    out->SetOutputModeMono(true);
    #else
    out = new AudioOutputI2SNoDAC();
            Serial.println("Using No DAC - using Serial port Rx pin");
    #endif
    out->SetGain(volume_level);
}


void MusicPlayer::play(String url){
    file_http = new AudioFileSourceHTTPStream();
    Serial.println(url);
    
    if ( file_http->open(url.c_str())) {
      buff = new AudioFileSourceBuffer(file_http, preallocateBuffer, preallocateBufferSize);
      mp3 = new AudioGeneratorMP3();
      mp3->begin(buff, out);
    } else {
      stopPlaying();
      Serial.println("error on open file");
    }
}


void MusicPlayer::loopTick(){
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

void MusicPlayer::stopPlaying(){
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