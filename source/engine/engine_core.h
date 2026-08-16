#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include "os/os_file_system.h"

namespace amit {
class EngineCore {
 public:
  static EngineCore* Instance();
  bool StartUp();
  void ShutDown();

    bool RegisterAssetPath(OSPath)

  /*
   bool StartUp(HINSTANCE hInstance, int windowLayout, bool isServer);
   void onNewFrame();
  bool isWindowClosed(HINSTANCE hInstance);
  
  SharedPointer<MessagingSystem> getMessagingSystem();
  SharedPointer<StringPool> getStringPool();
  SharedPointer<Windows::WindowingSystem> getWindowingSystem();
  SharedPointer<InputController> getInputController();
  SharedPointer<MouseController> getMouseInputController();*/
  

 private:
  EngineCore();
  // static SharedPointer<EngineCore> mEngineInstance;
  /*static SharedPointer<MessagingSystem> mMessagingSystem;
  static SharedPointer<StringPool> mStringPool;
  static SharedPointer<Windows::WindowingSystem> mEngineWindowingSystem;
  static SharedPointer<InputController> mInputController;
  static SharedPointer<MouseController> mMouseController;*/

};
}  

#endif
