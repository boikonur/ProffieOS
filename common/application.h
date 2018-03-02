#ifndef COMMON_APPLICATION_H
#define COMMON_APPLICATION_H

class Application : Looper, StateMachine {
public:
  const char* name() override { return "Application"; }
  void Loop() {
    STATE_MACHINE_BEGIN();
    SLEEP(1000);

    Looper::Unlink();
    STATE_MACHINE_END();
  }

private:

};
#endif
