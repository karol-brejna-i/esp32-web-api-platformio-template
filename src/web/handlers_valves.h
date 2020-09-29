#include <ESPAsyncWebServer.h>

void handleMainValveOn(AsyncWebServerRequest *request);
void handleMainValveOff(AsyncWebServerRequest *request);
void handleDrainValveOn(AsyncWebServerRequest *request);
void handleDrainValveOff(AsyncWebServerRequest *request);

void getValveState(String &result, int valve);
void handleMainValveGetState(AsyncWebServerRequest *request);
void handleDrainValveGetState(AsyncWebServerRequest *request);