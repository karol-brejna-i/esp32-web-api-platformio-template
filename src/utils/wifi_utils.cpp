#include "utils/wifi_utils.h"
#include "utils/gpio_utils.h"
#include "utils/debug_utils.h"

#include "ts.hpp"

#include "blink_task.h"
BlinkTask* blinkTask = new BlinkTask(&runner);


void waitForConnection()
{
    blinkTask->enable();
    while (WiFi.status() != WL_CONNECTED)
    {
        debugV("----Connecting to WiFi.."); // as RemoteDebug is not initialized at this moment probably we we'll need to use Serial.println
        Serial.println("Connecting to WiFi..");

        delay(1000);
        yield();
    }
    blinkTask->disable();
}

void setupWiFi(const char *ssid, const char *password)
{
    debugD("Setting up WiFi");
    WiFi.begin(ssid, password);
    waitForConnection();

    Serial.printf("Local IP: %s\n", WiFi.localIP().toString().c_str());
}
