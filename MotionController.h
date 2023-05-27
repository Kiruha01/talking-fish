#include <ArduinoJson.h>

#define BUFFER_LEN 1000

class MotionController{
  private:
    DynamicJsonDocument* jsonDoc;

  public:
    void setSmt(String cc){
      jsonDoc = new DynamicJsonDocument(900);
      DeserializationError err = deserializeJson(*jsonDoc, cc);
      if (err != DeserializationError::Ok){
        Serial.println("Error");
      }
      JsonArray head = (*jsonDoc)["h"];
      Serial.println(head[0].as<int>());
    }
};
