#ifndef CORE_GRAPHICS_FRAME_TIME_H_
#define CORE_GRAPHICS_FRAME_TIME_H_

#include <stdint.h>
#include <windows.h>

#include "../Utilities/RTTI.h"

namespace amit::graphics {
namespace time {
class FrameTime : public RTTI {
 public:
  static SharedPointer<FrameTime> getFrameTimeController();
  void pauseTimer(bool);
  bool getTimerStatus();
  float getdeltaTime();
  float getdeltaTimeDuringFrame();
  void updateDeltaTime();
  std::string getTypeInfo() const override { return ""; }
  bool isBothSameType(SharedPointer<RTTI>, std::string) const override {
    return true;
  }
  static uint64_t getCurrentTick();

 private:
  uint64_t lastTick;
  uint64_t currentTick;
  uint64_t pauseTimerTick;
  float deltaTime;
  bool isTimerPaused;
  uint64_t frameTimeFrequency;
  FrameTime(LARGE_INTEGER *, LARGE_INTEGER *);
};
}
}

#endif