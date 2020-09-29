#include <Arduino.h>
#include <ts.hpp>

#define DEFAULT_AUTOSWTICH_INTERVAL 15 * TASK_SECOND

#define FOREACH_VALVE_STATE(VALVE_STATE) \
        VALVE_STATE(VS_UNKNOWN)  \
        VALVE_STATE(VS_OPEN)     \
        VALVE_STATE(VS_CLOSED)    \
        VALVE_STATE(VS_OPENING)  \
        VALVE_STATE(VS_CLOSING)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum VALVE_STATE_ENUM {
    FOREACH_VALVE_STATE(GENERATE_ENUM)
};

class ValveController : public Task
{
public:
    VALVE_STATE_ENUM state;

    VALVE_STATE_ENUM previouisState;
    String lastAction;
    unsigned long lastActionTimestamp;

    // ValveController(Scheduler* aS, int gpioOpen, int gpioClose, unsigned long openingTime = DEFAULT_AUTOSWTICH_INTERVAL, unsigned long closingTime = DEFAULT_AUTOSWTICH_INTERVAL-100);
    ValveController(Scheduler* aS, String name, int gpioOpen, int gpioClose, unsigned long openingTime, unsigned long closingTime);
    ~ValveController() {};

    bool tryOpen();
    bool tryClose();
    String toString();
    String getName();
    String autoSwitchTaskToString();
    void setState(VALVE_STATE_ENUM newState);

    bool Callback();
    bool OnEnable();
    void OnDisable();

    // XXX TODO temporarly make this public
    void startAutoSwitch(unsigned long interval);
private:
    String name;
    int gpioClose;
    int gpioOpen;
    unsigned long openingTime;
    unsigned long closingTime;

    void transitionToOpen();
    void transitionToClose();
    void markPreviousState(String reason, unsigned long timestamp);

    void finishClosing();
    void finishOpening();

    static void switchOn(int pin);
    static void switchOff(int pin);

    // void startAutoSwitch(unsigned long interval);
};

extern ValveController* mainValve;
extern ValveController* drainValve;