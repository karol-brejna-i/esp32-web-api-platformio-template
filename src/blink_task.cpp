#include "blink_task.h"
#include "utils/debug_utils.h"
#include "utils/gpio_utils.h"


#define TIME_ON 100
#define TIME_OFF 1900

BlinkTask::BlinkTask(Scheduler* aS, bool switchOffOnDisable) : Task(TIME_OFF, TASK_FOREVER, aS, true) {
  debugD("BlinkTask constructor");
  this->switchOffOnDisable = switchOffOnDisable;
}

void BlinkTask::setState(bool aState)
{
  if (!aState) {
    this->state = 0;
    this->setInterval(TIME_OFF);
  } else {
    this->state = 1;
    this->setInterval(TIME_ON);
  }
}
bool BlinkTask::Callback() {
  debugV("blink callback");
  // dumpTaskInfo("blink callback", this);
  if (this->state == 0)
  {
    ledOn();
    setState(true);
  } else {
    ledOff();
    setState(false);
  }
  return true;
}

bool BlinkTask::OnEnable() {
  debugV("onenable blink");
  dumpTaskInfo("blink enabled", this);
  return true;
}

void BlinkTask::OnDisable() {
  debugV("ondisable blink");
  if (this->switchOffOnDisable) {
    ledOff();
  }
}