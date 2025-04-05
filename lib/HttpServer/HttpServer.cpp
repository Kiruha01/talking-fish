#include "HttpServer.h"



String configPage() {
    return "";
}

void HttpServer::begin() {

    server.begin();
}

void HttpServer::tick() {
    server.handleClient();
}

