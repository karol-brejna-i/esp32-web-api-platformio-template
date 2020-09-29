#include <ESPAsyncWebServer.h>
#include "utils/debug_utils.h"
#include "utils/wifi_utils.h"
#include "ts.hpp"

#include "web/handlers_update.h"
#include "web/handlers_ui.h"
#include "web/handlers_ws.h"
#include "web/handlers_status.h"
#include "web/handlers_valves.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setupHandlers()
{
    server.onNotFound(notFound);
    server.on("/ui", HTTP_GET, handleUI);
    server.on("/version", HTTP_GET, handleVersion);
    server.on("/status", HTTP_GET, handleStatus);

    server.on("/api/valve/main/open", HTTP_POST, handleMainValveOn);
    server.on("/api/valve/main/close", HTTP_POST, handleMainValveOff);
    server.on("/api/valve/main/getState", HTTP_GET, handleMainValveGetState);
    server.on("/api/valve/main/forceState", HTTP_POST, handleVersion); // XXX TODO

    server.on("/api/valve/drain/open", HTTP_POST, handleDrainValveOn);
    server.on("/api/valve/drain/close", HTTP_POST, handleDrainValveOff);
    server.on("/api/valve/drain/getState", HTTP_GET, handleDrainValveGetState);
    server.on("/api/valve/drain/forceState", HTTP_POST, handleVersion); // XXX TODO

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        debugD("/");
        request->send(200, "text/plain", "Hello, world");
    });

    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) { handleUpdate(request); });
    server.on("/doUpdate", HTTP_POST,
              [](AsyncWebServerRequest *request) {},
              [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                 size_t len, bool final) { handleDoUpdate(request, filename, index, data, len, final); });
}


void setupWebServer()
{
    debugI("Setting up a web server");

    setupHandlers();

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.begin();
}