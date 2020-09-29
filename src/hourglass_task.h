#include <ts.hpp>

class HourGlassTask : public Task {
  public:
    HourGlassTask(Scheduler* aS);
    ~HourGlassTask() {};

    bool Callback();
};

