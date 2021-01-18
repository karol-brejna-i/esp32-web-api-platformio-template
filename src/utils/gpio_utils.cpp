#include "gpio_utils.h"

#include "utils/debug_utils.h"

void setupGPIOs()
{
    // declare GIOs function
    pinMode(SIGNAL_LED, OUTPUT);
    pinMode(STER, OUTPUT);
    // digitalWrite(SIGNAL_LED, HIGH);
    // digitalWrite(STER, HIGH);
    digitalWrite(SIGNAL_LED, HIGH);
    digitalWrite(STER, LOW);
}

void powerOn()
{
    digitalWrite(STER, LOW);
}

void powerOff()
{
    digitalWrite(STER, HIGH);
}

void ledOn()
{
    debugD("LEDON");
    digitalWrite(SIGNAL_LED, LOW);
}

void ledOff()
{
    debugD("LEDOFF");
    digitalWrite(SIGNAL_LED, HIGH);
}

void press(int pressDelay)
{
    powerOn();
    delay(pressDelay);
    powerOff();
}

void blink(int blinkDelay)
{
    debugD("BLINK %d", blinkDelay);

    ledOn();
    delay(blinkDelay);
    ledOff();
}

void displayPinModes()
{
    debugD("Display pin modes");
    int cnt = 0;
    for (unsigned int pin = 0; pin < 32; pin++)
    {
        uint8_t pinomode2 = getPinMode((uint8_t)pin);
        if (pinomode2 == 1)
        {
            debugI("Mode of pin %d is %u", pin, pinomode2);
            cnt++;
        }
    }
    debugI("Output pins cnt %d", cnt);
}

#include <pins_arduino.h>
#define UNKNOWN_PIN 0xFF

uint8_t getPinMode(uint8_t pin)
{
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);

    // I don't see an option for mega to return this, but whatever...
    if (NOT_A_PIN == port)
        return UNKNOWN_PIN;

    // Is there a bit we can check?
    if (0 == bit)
        return UNKNOWN_PIN;

    // Is there only a single bit set?
    if (bit & (bit - 1))
        return UNKNOWN_PIN;

    volatile uint32_t *reg, *out;
    reg = portModeRegister(port);
    out = portOutputRegister(port);

    if (*reg & bit)
        return OUTPUT;
    else if (*out & bit)
        return INPUT_PULLUP;
    else
        return INPUT;
}