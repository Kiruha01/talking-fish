#include "ESP8266WebServer.h"

class HttpServer
{
private:
    ESP8266WebServer server = ESP8266WebServer(80);
public:
    void begin();
    void tick();
};
