// CApp.h - the application class, it is a singleton

#ifndef AERO_SIMULATOR_CAPP_H
#define AERO_SIMULATOR_CAPP_H

#include "CAppWindow.h"
#include "CTaskManager.h"

#include <iostream>
#include <memory>

namespace AeroSimulatorEngine
{
   class CApp
   {
   public:
      static CApp& getInstance()
      {
         static CApp instance;
         return instance;
      }

      ~CApp();

      ///@todo: later add passing app parameters read from a file here
      bool init(const char* name, unsigned int width, unsigned int height);
      void run();

   private:
      CApp();
      CApp(const CApp&) = delete;
      void operator=(const CApp&) = delete;

      // A factory method, returns a window suitable for the current platform
      std::shared_ptr<CAppWindow> createAppWindow();

   private:
      CTaskManager mTaskManager;
      std::shared_ptr<CAppWindow> mAppWindowTask;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPP_H
