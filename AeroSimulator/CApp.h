// CApp.h - the application class, it manages the app window and tasks.
// Implemented as singleton.

#ifndef AERO_SIMULATOR_CAPP_H
#define AERO_SIMULATOR_CAPP_H

#include "CTaskManager.h"

#include <iostream>
#include <memory>
#include <vector>

#define APP CApp::getInstance()

///@todo: rename to NAeroSimulatorEngine
namespace AeroSimulatorEngine
{
   class CWin32Window;
   class CWin32Renderer;
   class CRenderer;
   class CTimer;

   class CApp
   {
   public:
      static CApp & getInstance()
      {
         static CApp instance;
         return instance;
      }

      ~CApp();

      ///@todo: later add passing app parameters read from a config file here
      bool init(const char* name, unsigned int width, unsigned int height);
      void run();
      int exit();

      void addTask(CTask * pTask);

      ///@todo: think how to avoid exposing a private member
      CRenderer * getRenderer() const;

   private:
      CApp();
      // Disable the copy constructor and copy assignment operator
      CApp(const CApp &) = delete;
      void operator=(const CApp &) = delete;

   private:

      ///@todo: introduce a Bridge pattern and place Win32-specific code there
      CTaskManager mTaskManager;

      ///@todo: reconsider this: use unique_ptr if the object is not shared
      std::shared_ptr<CWin32Window> mAppWindowTask;
      std::shared_ptr<CWin32Renderer> mRendererTask;
      std::shared_ptr<CTimer> mTimerTask;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPP_H
