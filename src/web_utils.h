#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;
extern AsyncWebSocket ws;
extern AsyncWebSocketClient *globalClient;

void setupWebServer();