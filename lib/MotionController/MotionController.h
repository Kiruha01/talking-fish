#include <ArduinoJson.h>

#define BUFFER_LEN 3000


class MotionController{
  private:
    JsonDocument* jsonDoc;
    
    byte headPin;
    byte tailPin;
    byte mouthPin;
    
    int head[100];
    int currentHeadIndex = 0;
    int headLen = 0;
    bool headState = false;
    
    int tail[100];
    int tailLen = 0;
    int currentTailIndex = 0;
    bool tailState = false;
    
    int mouth[500];
    int mouthLen = 0;
    int currentMouthIndex = 0;
    bool mouthState = false;

    unsigned long startTime;
    bool started = false;
    bool wait_motion = true;

    void toogleHead();
    void toogleTail();
    void toogleMouth();
  public:
    MotionController(byte hPin, byte tPin, byte mPin) : headPin(hPin), tailPin(tPin), mouthPin(mPin) {};
    void setMotion(String cc);
    void start();
    void loopTick();
};