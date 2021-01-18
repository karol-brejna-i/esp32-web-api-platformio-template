#include "handlers_status.h"
#include "utils/debug_utils.h"
#include "blink_task.h"
#include "valves.h"
#include "build.h"

#include "utils/gpio_utils.h"

void handleVersion(AsyncWebServerRequest *request)
{
    debugD("handleVersion");
    request->send(200, "application/json", VERSION_JSON);
}

BlinkTask* statusBlink = NULL;
ValveController* testValveController = NULL;

void handleStatus(AsyncWebServerRequest *request)
{
    debugI("handleStatus");
    // TODO XXX create getHostname util function
    #ifdef ESP32
    String hostname = WiFi.getHostname();
    #else
    String hostname = WiFi.hostname();
    #endif
    int strength = WiFi.RSSI();

    if (statusBlink == NULL) {
        debugV("creatubg blinkTask instance");
        statusBlink = new BlinkTask(&runner);
        statusBlink->disable();
    }

    if (statusBlink->isEnabled()) {
        statusBlink->disable();
    } else {
        statusBlink->setIterations(10);
        statusBlink->enable();
    }

    // XXX TODO make it prettier
    String mainValveStatus = mainValve->toString();
    String drainValveStatus = drainValve->toString();

    String status = String("hostname: " + hostname + ", strength: " + strength + ", mainValve: " + mainValveStatus + ", drainValve: " + drainValveStatus);

    debugD("<---------------------------------------------------------------");
    debugD("%s\nTaskID: %u; interval: %lu; isEnabled: %d; runCounter: %lu; iterations: %ld; %lu;\n",
            "main valve:", mainValve->getId(), mainValve->getInterval(), mainValve->isEnabled(), mainValve->getRunCounter(), mainValve->getIterations(), millis());
    debugD("%s\nTaskID: %u; interval: %lu; isEnabled: %d; runCounter: %lu; iterations: %ld; %lu;\n",
            "drain valve:", drainValve->getId(), drainValve->getInterval(), drainValve->isEnabled(), drainValve->getRunCounter(), drainValve->getIterations(), millis());
    debugD("status response: %s", status.c_str());
    debugD(">---------------------------------------------------------------");

    displayPinModes();
    request->send(200, "text/html", status);
}
