#ifndef ENGINE_INPUT_INPUT_DEFS_H_
#define ENGINE_INPUT_INPUT_DEFS_H_

#include "engine_core.h"

namespace amit {
struct MouseStates {
  bool leftButtonPressed;
  bool rightButtonPressed;
  bool middleButtonPressed;
  bool shiftKey;
  bool ctrlKey;
  bool mouseMoving;
  MouseStates() {
    leftButtonPressed = rightButtonPressed = middleButtonPressed = shiftKey =
        ctrlKey = mouseMoving = false;
  }
};

struct MouseEventAndPosition {
  int x;     // x-coordinate
  int y;     // y-coordinate
  POINTS P;  // Point in screen coordinate - depending on screen size
  MouseStates mMouseStates;
};

struct WindowsParam {
  UINT windowsMessage;
  WPARAM wParam;
  LPARAM lParam;
};
}  // namespace amit
#endif
