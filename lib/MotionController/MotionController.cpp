#include "MotionController.h"
#include "config.h"

void MotionController::toogleHead(){
    #ifdef DEBUG_FLAG
    Serial.print("HEAD STATE: ");
    Serial.println(!headState);
    #endif
    headState = !headState;
    digitalWrite(headPin, headState);
    digitalWrite(tailPin, LOW);
  }

void MotionController::toogleTail(){
    #ifdef DEBUG_FLAG
    Serial.print("TAIL STATE: ");
    Serial.println(!tailState);
    #endif
    tailState = !tailState;
    digitalWrite(tailPin, tailState);
    digitalWrite(headPin, LOW);
  }

void MotionController::toogleMouth(){
    #ifdef DEBUG_FLAG
    Serial.print("MOUTH STATE: ");
    Serial.println(!mouthState);
    #endif
    mouthState = !mouthState;
    digitalWrite(mouthPin, mouthState);
  }


void MotionController::setMotion(String cc){
    jsonDoc = new JsonDocument(); // TODO: on json size
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
     wait_motion = false;

  }

void MotionController::start(){
    currentHeadIndex = 0;
    currentTailIndex = 0;
    currentMouthIndex = 0;
    started = false;
  }


void MotionController::loopTick(){
    if (wait_motion) return;
    
    if (!started){
      startTime = millis();
      started = true;
      return;
    }

    unsigned long delayTime = millis() - startTime;
    Serial.println(delayTime);
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

    if (currentMouthIndex >= mouthLen and currentTailIndex >= tailLen and currentHeadIndex >= headLen){
      started = false;
      wait_motion = true;
    }

  }

  