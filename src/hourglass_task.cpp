#include "hourglass_task.h"
#include "utils/debug_utils.h"

HourGlassTask::HourGlassTask(Scheduler* aS) : Task(2 * TASK_SECOND, TASK_FOREVER, aS, true) {
  debugD("HourGlassTask constructor");
}

bool HourGlassTask::Callback() {
  debugD(".");
  return true;
}
