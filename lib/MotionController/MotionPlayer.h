#pragma once
#include <ArduinoJson.h>
#include "HttpSpecFinder.h"


class MotionPlayer{
  private:
    MotionSpec* currentSpec = nullptr;
    size_t currentHeadIndex = 0;
    bool headState = false;
    
    size_t currentTailIndex = 0;
    bool tailState = false;
    
    size_t currentMouthIndex = 0;
    bool mouthState = false;

    unsigned long startTime;
    bool started = false;
    bool wait_motion = true;

    void toogleHead();
    void toogleTail();
    void toogleMouth();
  public:
    void setMotion(MotionSpec* spec);
    void start();
    void loopTick();
    ~MotionPlayer() {
      if (currentSpec) {
          delete currentSpec;
          currentSpec = nullptr;
      }
  }
};