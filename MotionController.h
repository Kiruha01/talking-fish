#include <ArduinoJson.h>

#define BUFFER_LEN 1000


class MotionController{
  private:
    DynamicJsonDocument* jsonDoc;
    
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

    void toogleHead(){
      #ifdef DEBUG_FLAG
      Serial.print("HEAD STATE: ");
      Serial.println(!headState);
      #endif
      headState = !headState;
      digitalWrite(HEAD_PIN, headState);
      digitalWrite(TAIL_PIN, LOW);
    }
    void toogleTail(){
      #ifdef DEBUG_FLAG
      Serial.print("TAIL STATE: ");
      Serial.println(!tailState);
      #endif
      tailState = !tailState;
      digitalWrite(TAIL_PIN, tailState);
      digitalWrite(HEAD_PIN, LOW);
    }
    void toogleMouth(){
      #ifdef DEBUG_FLAG
      Serial.print("MOUTH STATE: ");
      Serial.println(!mouthState);
      #endif
      mouthState = !mouthState;
      digitalWrite(MOUTH_PIN, mouthState);
    }

  public:
    void setMotion(String cc){
      jsonDoc = new DynamicJsonDocument(BUFFER_LEN); // TODO: on json size
      DeserializationError err = deserializeJson(*jsonDoc, cc);
      if (err != DeserializationError::Ok){
        Serial.println("Error");
        Serial.println(err.f_str());
      }
        if ((*jsonDoc)["h"].is<JsonArray>()) {
          JsonArray s = (*jsonDoc)["h"];
          headLen = s.size();
          for (int i = 0; i<headLen; ++i){
            Serial.print(s[i].as<int>());
            Serial.print(" ");
            head[i] = s[i].as<int>();
          }
        }
       Serial.println();
        if ((*jsonDoc)["t"].is<JsonArray>()) {
          JsonArray s = (*jsonDoc)["t"];
          tailLen = s.size();
          for (int i = 0; i<tailLen; ++i){
            Serial.print(s[i].as<int>());
            Serial.print(" ");
            tail[i] = s[i].as<int>();
          }
        }
       Serial.println();
       if ((*jsonDoc)["m"].is<JsonArray>()) {
          JsonArray s = (*jsonDoc)["m"];
          mouthLen = s.size();
          for (int i = 0; i<mouthLen; ++i){
            Serial.print(s[i].as<int>());
            Serial.print(" ");
            mouth[i] = s[i].as<int>();
          }
        }
       Serial.println();

    }

    void start(){
      startTime = millis();
      currentHeadIndex = 0;
      currentTailIndex = 0;
      currentMouthIndex = 0;
    }

    void loopTick(){

      unsigned long delayTime = millis() - startTime;
      if (delayTime >= head[currentHeadIndex] and currentHeadIndex < headLen){
        toogleHead();
          ++currentHeadIndex;
      }
      if (delayTime >= tail[currentTailIndex] and currentTailIndex < tailLen){
        toogleTail();
        ++currentTailIndex;
      }
      if (delayTime >= mouth[currentMouthIndex] and currentMouthIndex < mouthLen){
        toogleMouth();
        ++currentMouthIndex;
      }

    }

    
};
