#ifndef __INPUT_CONTROLLER_H
#define __INPUT_CONTROLLER_H

#include "../../Utilities/IMessageHandler.h"
#include "../../Utilities/RTTI.h"
#include "../../Utilities/SharedPointer.h"

namespace amit {
class InputController : public IMessageHandler {
 public:
  static SharedPointer<InputController> getInputController();
  void HandleMessage(utils::StringHash&, SharedPointer<RTTI>,
                     void* i_pMessageData) override;
  std::string GetType() const override;
  bool IsSameType(SharedPointer<RTTI>, std::string) const override;

 private:
  static SharedPointer<InputController> mInputController;
  std::string mTypeName;
  InputController();
};
}  

#endif