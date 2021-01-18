#include "handlers_update.h"
#include "utils/debug_utils.h"


size_t content_len;

void handleDoUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        debugV("Update");
        content_len = request->contentLength();
        // if filename includes spiffs, update the spiffs partition
        int cmd = (filename.indexOf("spiffs") > -1) ? U_PART : U_FLASH;
#ifdef ESP8266
        Update.runAsync(true);
        if (!Update.begin(content_len, cmd))
        {
#else
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
        {
#endif
            Update.printError(Serial);
        }
    }

    if (Update.write(data, len) != len)
    {
        Update.printError(Serial);
#ifdef ESP8266
    }
    else
    {
        debugD("Progress: %d%%\n", (Update.progress() * 100) / Update.size());
#endif
    }

    if (final)
    {
        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Please wait while the device reboots");
        response->addHeader("Refresh", "20");
        response->addHeader("Location", "/");
        request->send(response);
        if (!Update.end(true))
        {
            Update.printError(Serial);
        }
        else
        {
            debugV("Update complete");
            Serial.flush();
            ESP.restart();
        }
    }
}
