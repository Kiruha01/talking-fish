#include "MotionPlayer.h"

void MotionPlayer::toogleHead() {
    #ifdef DEBUG_FLAG
    Serial.print("HEAD STATE: ");
    Serial.println(!headState);
    #endif
    headState = !headState;
    digitalWrite(HEAD_PIN, headState);
    digitalWrite(TAIL_PIN, LOW);
  }

void MotionPlayer::toogleTail() {
    #ifdef DEBUG_FLAG
    Serial.print("TAIL STATE: ");
    Serial.println(!tailState);
    #endif
    tailState = !tailState;
    digitalWrite(TAIL_PIN, tailState);
    digitalWrite(HEAD_PIN, LOW);
  }

void MotionPlayer::toogleMouth() {
    #ifdef DEBUG_FLAG
    Serial.print("MOUTH STATE: ");
    Serial.println(!mouthState);
    #endif
    mouthState = !mouthState;
    digitalWrite(MOUTH_PIN, mouthState);
  }


void MotionPlayer::setMotion(MotionSpec* spec) {
  if (currentSpec != nullptr) delete currentSpec;

  currentSpec = spec;
  currentHeadIndex = 0;
  currentTailIndex = 0;
  currentMouthIndex = 0;
  wait_motion = false;

  }

void MotionPlayer::start(){
    currentHeadIndex = 0;
    currentTailIndex = 0;
    currentMouthIndex = 0;
    headState = false;
    tailState = false;
    mouthState = false;
    digitalWrite(HEAD_PIN, LOW);
    digitalWrite(TAIL_PIN, LOW);
    digitalWrite(MOUTH_PIN, LOW);
    started = false;
  }


void MotionPlayer::loopTick(){
    if (wait_motion) return;
    if (currentSpec == nullptr) return;
    
    if (!started){
      startTime = millis();
      started = true;
      return;
    }

    unsigned long delayTime = millis() - startTime;
    
    #ifdef DEBUG_FLAG
    Serial.printf("h %d t %d m %d\n", currentHeadIndex, currentTailIndex, currentMouthIndex);
    #endif

    if (delayTime >= currentSpec->head[currentHeadIndex] && currentHeadIndex < currentSpec->headSize){
      toogleHead();
      ++currentHeadIndex;
    }

    if (delayTime >= currentSpec->tail[currentTailIndex] && currentTailIndex < currentSpec->tailSize){
      toogleTail();
      ++currentTailIndex;
    }

    if (delayTime >= currentSpec->mouth[currentMouthIndex] && currentMouthIndex < currentSpec->mouthSize){
      toogleMouth();
      ++currentMouthIndex;
    }

    if (currentMouthIndex >= currentSpec->mouthSize && currentTailIndex >= currentSpec->tailSize && currentHeadIndex >= currentSpec->headSize){
      started = false;
      wait_motion = true;
    }
  }

  