// TODO XXX: I thnk you overdid it as far as the includes are concerned. clean it up

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <Update.h>
#define U_PART U_SPIFFS
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <Updater.h>
#define U_PART U_FS
#endif
#include <ESPAsyncWebServer.h>


void handleDoUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);