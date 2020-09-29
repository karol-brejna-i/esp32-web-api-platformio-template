#include "valves.h"
#include "utils/debug_utils.h"

static const char *VALVE_STATE_STRING[] = {
    FOREACH_VALVE_STATE(GENERATE_STRING)};

ValveController::ValveController(Scheduler* aS, String name, int gpioClose, int gpioOpen, unsigned long openingTime, unsigned long closingTime)
                        : Task(TASK_MINUTE * 60, TASK_FOREVER, aS, false)
{
    debugI("I am in ValveController constructor.");
    debugI("String name, int gpioClose, int gpioOpen, unsigned long openingTime, unsigned long closingTime, %s %d %d %lu %lu",
        name.c_str(), gpioClose, gpioOpen, openingTime, closingTime);

    this->name = String(name);
    this->setState(VS_UNKNOWN);

    this->gpioOpen = gpioOpen;
    this->gpioClose = gpioClose;

    this->openingTime = openingTime;
    this->closingTime = closingTime;
    // XXX TODO here goes the dilema: do we assume that vavlecontroller gets gpio pins already configured?
    // or do we configure them ourselves?
    // XXX TODO also, for some applications ON may mean Low for other High...
}


void ValveController::setState(VALVE_STATE_ENUM newState)
{
    this->state = newState;
}

void ValveController::markPreviousState(String reason, unsigned long timestamp = millis())
{
    this->previouisState = state;
    this->lastActionTimestamp = timestamp;
    this->lastAction = reason;
}

String ValveController::getName() {
    return this->name;
}

String ValveController::toString()
{
    char buffer[168];
    snprintf(buffer, 164, "{\"name\": \"%s\", \"state\": \"%s\", \"lastAction\": \"%s\", \"lastActionTimestamp\": \"%ld\"}",
        this->name.c_str(), VALVE_STATE_STRING[this->state], this->lastAction.c_str(), this->lastActionTimestamp);
    return String(buffer);
}

String ValveController::autoSwitchTaskToString()
{
    char buffer[102];
    snprintf(buffer, 100, "ValveController {state: %s, lastAction: %s, lastActionTimestamp: %ld}", VALVE_STATE_STRING[this->state], this->lastAction.c_str(), this->lastActionTimestamp);
    return String(buffer);

}

void ValveController::switchOn(int pin)
{
    debugD("switchOn %d", pin);
    digitalWrite(pin, HIGH);
}

void ValveController::switchOff(int pin)
{
    debugD("switchOff %d", pin);
    digitalWrite(pin, LOW);
}

/**
 * beacause TaskScheduler lib is "hard" to use with objects (cannot set object's method as a callback, task's callback has no access to method properties... )
 * here we try to use objects inherited from Task. that objects need to provide Callback method. the method cannot be changed duging object life cycle.
 * so, here either I need a callback that is aware of context/status... i'd prefer to have a dedicated task/callback for finishing closing and finishing opening
 * so I am sure they'll do what needs to bve done. now, if something is wrong with the state/context is wrong -- auto switch may not work.
 *
 * I just thought that autoswitch should switch off all the relays just in case and depend on the state only for determiining new state (opened/closed)
 */
bool ValveController::Callback() {
    debugD("callback %s", this->name.c_str());


    debugD("callback %s; state: %s, lastAction: %s, lastActionTimestamp: %lu}", this->name.c_str(), VALVE_STATE_STRING[this->state], this->lastAction.c_str(), this->lastActionTimestamp);
    // switch off the relays
    switchOff(gpioOpen);
    switchOff(gpioClose);

    // determine new state
    switch (state)
    {
    case VS_CLOSING:
        debugI("Autoswitch triggered on VS_Closing. Cool.");
        state = VS_CLOSED;
        break;
    case VS_CLOSED:
        state = VS_CLOSED;
        debugW("Autoswitch triggered on VS_CLOSED rely. It should only happen for VS_CLOSING state.");
        break;
    case VS_OPENING:
        debugI("Autoswitch triggered on VS_OPENING. Cool.");
        state = VS_OPEN;
        break;
    case VS_OPEN:
        state = VS_OPEN;
        debugW("Autoswitch triggered on VS_OPEN rely. It should only happen for VS_OPENING state.");
        break;
    case VS_UNKNOWN:
        state = VS_UNKNOWN;
        debugE("Autoswitch triggered on VS_UNKNOWN rely.");
        break;
    default:
        debugE("-#--#--#- I should never get here!");
        break;
    }
    return true;
}

void ValveController::startAutoSwitch(unsigned long interval)
{
    debugD("startAutoSwitch %s %lu", this->name.c_str() ,interval);
    debugD("%s\nTask ID: %u; interval: %lu; isEnabled: %d; runCounter: %lu; iterations: %ld; %lu;\n",
            "startautoswitch task before:", this->getId(), this->getInterval(), this->isEnabled(), this->getRunCounter(), this->getIterations(), millis());

    this->setInterval(interval);
    this->setIterations(1);
    this->enableDelayed(); // this->enable(); wouldn't work. the callback was invoked immediately.
    debugD("%s\nTask ID: %u; interval: %lu; isEnabled: %d; runCounter: %lu; iterations: %ld; %lu;\n",
            "startautoswitch task after:", this->getId(), this->getInterval(), this->isEnabled(), this->getRunCounter(), this->getIterations(), millis());
}

// XXX TODO tryopen rename to startOpening?
bool ValveController::tryOpen()
{
    debugD("tryOpen");
    markPreviousState("TRY OPEN");
    debugV("set state to OPNENING");
    state = VS_OPENING;

    debugV("make sure the valve newer tries to open and close at the same time");
    // 0. switch off GPIO_CLOSE (make sure the valve newer tries to open and close at the same time) -- maybe dedicated method "finishClosing" will do that?
    switchOff(this->gpioClose);
    // 1. turn on GPIO_OPEN
    switchOn(this->gpioOpen);

    // 3. start auto-off timer with callback to finishOpening (depends on current state - vs_opening)
    debugD("Starting auto switch...");
    startAutoSwitch(this->openingTime);
    debugD("after startAutoSwitch %lu", this->openingTime);

    return true;
}

bool ValveController::tryClose()
{
    debugV("tryClose");
    markPreviousState("TRY CLOSE");

    debugV("set state to CLOSING");
    state = VS_CLOSING;

    debugV("make sure the valve newer tries to open and close at the same time");
    // 0. switch off GPIO_OPEN (make sure the valve newer tries to open and close at the same time) -- maybe dedicated method "finishClosing" will do that?
    switchOff(this->gpioOpen);
    // 1. turn on GPIO_CLOSE
    switchOn(this->gpioClose);

    // 3. start auto-off timer with callback to finishClosing (depends on current state - vs_closing)
    debugD("Starting auto switch...");
    startAutoSwitch(this->closingTime);
    debugD("after startAutoSwitch %lu", this->closingTime);
    return true;
}

void ValveController::finishClosing()
{
    // turn off GPIO_CLOSE
}

void ValveController::transitionToOpen()
{
    debugD("---}}} Transition to ON");
    // disable autoSwitchTask
    // set proper state (opened)
    // disable gpio
}

void ValveController::transitionToClose()
{
    debugD("---}}} Transition to OFF");

    // disable autoSwitchTask
    // set proper state (opened)
    // disable gpio
}


bool ValveController::OnEnable() {
  debugD("onenable ValveController %s", this->name.c_str());
  debugD("%s\nTask ID: %u; interval: %lu; isEnabled: %d; runCounter: %lu; iterations: %ld; %lu;\n",
    "onenable task status:", this->getId(), this->getInterval(), this->isEnabled(), this->getRunCounter(), this->getIterations(), millis());
  return true;
}

void ValveController::OnDisable() {
  debugD("ondisable ValveController %s", this->name.c_str());
}