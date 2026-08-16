#ifndef __MOUSE_CONTROLLER_H
#define __MOUSE_CONTROLLER_H

#include "../../Utilities/IMessageHandler.h"
#include "../../Utilities/RTTI.h"
#include "../../Utilities/SharedPointer.h"
#include "../enginedefs.h"

namespace amit {
class MouseController : public IMessageHandler {
 public:
  static SharedPointer<MouseController> getMouseController();
  MouseEventAndPosition getCurrentMouseState() const;
  void HandleMessage(utils::StringHash&, SharedPointer<RTTI>,
                     void* i_pMessageData) override;
  std::string GetType() const override;
  bool IsSameType(SharedPointer<RTTI>, std::string) const override;
  void leftButtonPressed(bool);
  void rightButtonPressed(bool);
  void middleButtonPressed(bool);
  void mouseMoving(bool);
  void setMousePosition(int, int, POINTS);

 private:
  static SharedPointer<MouseController> mMouseInputController;
  std::string mTypeName;
  MouseEventAndPosition mMouseCurrentStateAndPosition;
  MouseEventAndPosition mMousePreviousStateAndPosition;
  MouseController();
  bool arcBallReady;
};
}  
#endif
