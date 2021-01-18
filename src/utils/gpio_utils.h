#define LED_BUILTIN 4


// PILOT MA PO PRAWEJ MINUS WE WŁĄCZNIKU
#define RED_LED 33 //GPIO33
#define GPIO_13 13
#define GPIO_12 12
#define GPIO_05 5
#define GPIO_04 4
#define GPIO_02 2

#define GPIO_16 16

// #define STER GPIO_16
#define STER LED_BUILTIN
#define SIGNAL_LED RED_LED
#define PRESS_DELAY 4000
#define FLASH_DELAY 100


// XXX in progress, wiring the valves
#define MAIN_VALVE_OPEN     GPIO_16 //D1
#define MAIN_VALVE_CLOSE    GPIO_16 //D2
#define DRAIN_VALVE_OPEN    GPIO_16 //D4
#define DRAIN_VALVE_CLOSE   3 //GPIO02

void setupGPIOs();

void press(int pressDelay = PRESS_DELAY);
void blink(int blinkDelay = FLASH_DELAY);

void powerOn();
void powerOff();
void ledOn();
void ledOff();

#include  <stdint.h>

uint8_t getPinMode(uint8_t pin);
void displayPinModes();