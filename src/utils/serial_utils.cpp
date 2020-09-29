#include <Arduino.h>

void setupSerial()
{
    Serial.begin(115200);
    Serial.println("starting the setup");
    Serial.setDebugOutput(true);
    Serial.println("Serial setup ready");
}