#include <ts.hpp>

class BlinkTask : public Task {
  public:
    BlinkTask(Scheduler* aS, bool switchOffOnDisable=true);
    ~BlinkTask() {};

    bool Callback();
    bool OnEnable();
    void OnDisable();
    void setState(bool aState);
  private:
    int state; // 0 off, 1 on
    bool switchOffOnDisable;

};

