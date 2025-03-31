#pragma once

#include "AudioFileSourceHTTPStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"

#ifdef USE_I2S
#include "AudioOutputI2S.h"
#else
#include "AudioOutputI2SNoDAC.h"
#endif



class MusicPlayer{
  private:
    String musicUrl;
    AudioGeneratorMP3         *mp3 = NULL;
    AudioFileSourceHTTPStream *file_http = NULL;
    AudioFileSourceBuffer     *buff = NULL;
    #ifdef USE_I2S
    AudioOutputI2S            *out = NULL;
    #else
    AudioOutputI2SNoDAC       *out = NULL;
    #endif
    const int preallocateBufferSize = 4096;
    void *preallocateBuffer = NULL;

  public:
    MusicPlayer();
  
    void play(String url);
    void loopTick();
    void stopPlaying();

    ~MusicPlayer() {
      stopPlaying();
      if (out) {
          delete out;
          out = nullptr;
      }
      if (preallocateBuffer) {
          free(preallocateBuffer);
          preallocateBuffer = nullptr;
      }
  }

};