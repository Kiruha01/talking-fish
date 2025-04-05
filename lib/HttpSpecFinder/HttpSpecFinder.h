#pragma once

#include "ArduinoJson.h"
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

struct MotionSpec {
    unsigned long* head;
    size_t headSize;
    unsigned long* tail;
    size_t tailSize;
    unsigned long* mouth;
    size_t mouthSize;

};


class HttpSpecFinder {
private:
    String baseUrl;

    MotionSpec* convertStringToSpec(String s) {
        JsonDocument jsonDoc;

        DeserializationError err = deserializeJson(jsonDoc, s);
        if (err != DeserializationError::Ok){
            return nullptr;
        }
        MotionSpec* spec = new MotionSpec();

        if (jsonDoc["h"].is<JsonArray>()) {
            spec->headSize = jsonDoc["h"].size();
            spec->head = new unsigned long[spec->headSize];

            for (size_t i = 0; i < spec->headSize; ++i){
                spec->head[i] = jsonDoc["h"][i].as<unsigned long>();

            }
        }
        if (jsonDoc["t"].is<JsonArray>()) {
            spec->tailSize = jsonDoc["t"].size();
            spec->tail = new unsigned long[spec->tailSize];

            for (size_t i = 0; i < spec->tailSize; ++i){
                spec->tail[i] = jsonDoc["t"][i].as<unsigned long>();

            }
        }
        if (jsonDoc["m"].is<JsonArray>()) {
            spec->mouthSize = jsonDoc["m"].size();
            spec->mouth = new unsigned long[spec->mouthSize];

            for (size_t i = 0; i < spec->mouthSize; ++i){
                spec->mouth[i] = jsonDoc["m"][i].as<unsigned long>();

            }
        }

        return spec;
    }

public:
    void setBaseUrl(const String& url) {
        baseUrl = url;
    }

    MotionSpec* fetchSpec(String specName) {
        if (baseUrl == nullptr) return nullptr;

        WiFiClient client;
        HTTPClient http;

        if (!http.begin(client, baseUrl + "/" + specName + ".json")) return nullptr;
        if (http.GET() != HTTP_CODE_OK) {
            http.end();
            return nullptr;
        } 
        String payload = http.getString();
        http.end();
        
        return convertStringToSpec(payload);
    }

    int checkStatusCode(String filename) {
        if (baseUrl == nullptr) return 0;

        WiFiClient client;
        HTTPClient http;
        Serial.println(baseUrl + "/" + filename);
        if (!http.begin(client, baseUrl + "/" + filename)) return 0;
        int status = http.sendRequest("HEAD");
        http.end();
        return status;
    }

    String makeMp3Link(String name) {
        return baseUrl + "/" + name + ".mp3";
    }

    ~HttpSpecFinder() {}
};

