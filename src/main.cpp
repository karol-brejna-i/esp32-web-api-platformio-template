#include <Arduino.h>

#include "build.h"
#include "utils/debug_utils.h"
#include "utils/serial_utils.h"
#include "utils/gpio_utils.h"
#include "utils/wifi_utils.h"
#include "web_utils.h"
#include "ts.hpp"

#ifndef WIFI_SSID
#error wifi not defined
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "My ssid name"
#define WIFI_PASS "My password"
#endif

#ifndef DEBUGGER_ENABLED
error "debuger enabled"
#endif

    RemoteDebug Debug;

Scheduler runner;
// debug task;
#include "hourglass_task.h"
HourGlassTask *hourglassTask = new HourGlassTask(&runner);

#include "valves.h"
ValveController *mainValve = new ValveController(&runner, "main", MAIN_VALVE_OPEN, MAIN_VALVE_CLOSE, DEFAULT_AUTOSWTICH_INTERVAL, DEFAULT_AUTOSWTICH_INTERVAL);
ValveController *drainValve = new ValveController(&runner, "drain", DRAIN_VALVE_OPEN, DRAIN_VALVE_CLOSE, DEFAULT_AUTOSWTICH_INTERVAL, DEFAULT_AUTOSWTICH_INTERVAL);

void processCmdRemoteDebug()
{

    String lastCmd = Debug.getLastCommand();
    debugI("debug command %s", lastCmd.c_str());

    uint8_t mode = 255;
    String pin;
    if (lastCmd.startsWith("input"))
    {
        pin = lastCmd.substring(5);
        mode = INPUT;
    }
    else if (lastCmd.startsWith("output"))
    {
        pin = lastCmd.substring(6);
        mode = 1;
    }
    else
    {
        debugW("kupa");
    }

    debugI("pin %s, mode %u", pin.c_str(), mode);

    if (mode != 255)
    {

        int pinNo = pin.toInt();
        debugI("changing pin %d state...", pinNo);
        pinMode(pinNo, mode);
    }
    displayPinModes();
}

void setup()
{
    setupSerial();
    Serial.println("starting the setup process");
#ifdef DEBUG_ON
    Serial.print("Starting..........");
    Serial.println(VERSION_JSON);
#endif

    setupGPIOs();
    setupWiFi(WIFI_SSID, WIFI_PASS);
    setupWebServer();

    // Initialize RemoteDebug // TODO move to a dedicated function/file
    String hostname = WiFi.hostname();
    Debug.setResetCmdEnabled(true);
    // Debug.showProfiler(true);
    Debug.showColors(true);
    Debug.showTime(true);
    Debug.setSerialEnabled(true);
    Debug.begin(hostname, Debug.INFO);

    String helpCmd = "inputX -- where X is GPIO number (i.e.1, 2, 6, 9, 20) -- set given GPIO to INPUT mode\n";
    helpCmd.concat("outputX -- where X is GPIO number (i.e.1, 2, 6, 9, 20) -- set given GPIO to OUTPUT mode");

    Debug.setHelpProjectsCmds(helpCmd);
    Debug.setCallBackProjectCmds(&processCmdRemoteDebug);

    Serial.println("Setup process finished...");
}

void loop()
{
    runner.execute();
    // delay(10);

    Debug.handle();
    yield();
}