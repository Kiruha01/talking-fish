#pragma once
#include "MotionPlayer.h"
#include "MusicPlayer.h"
#include "HttpSpecFinder.h"

class MotionController
{
private:
    MotionPlayer motionPlayer;
    MusicPlayer musicPlayer;
public:
    void setUp() {
        pinMode(HEAD_PIN, OUTPUT);
        pinMode(TAIL_PIN, OUTPUT);
        pinMode(MOUTH_PIN, OUTPUT);
    }

    HttpSpecFinder specFinder;
    void tick() {
        musicPlayer.loopTick();
        motionPlayer.loopTick();
    }

    void setUpSpec(String name) {
        if (specFinder.checkStatusCode(name + ".mp3") != 200){
            Serial.println("not found");
            return;
        }
    
        MotionSpec* newSpec = specFinder.fetchSpec(name);
        if (newSpec == nullptr) return;

        #ifdef DEBUG_FLAG
        Serial.print("head spec size: ");
        for (size_t i = 0; i < newSpec->headSize; ++i){
            Serial.printf("%ld ", newSpec->head[i]);
        }
        #endif
        Serial.println();
        musicPlayer.play(specFinder.makeMp3Link(name));
        motionPlayer.setMotion(newSpec);
        motionPlayer.start();
    }
};