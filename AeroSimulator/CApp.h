//#pragma once

#ifndef AERO_SIMULATOR_CAPP_H
#define AERO_SIMULATOR_CAPP_H

#include <iostream>
#include <memory>
#include "CAppWindow.h"

//namespace AeroSimulatorEngine
//{
// TODO: make it a singleton
   class CApp
   {
   public:
      CApp();
      ~CApp();

      // TODO: later add passing app parameters read from a file here
      bool init(const char* name, unsigned int width, unsigned int height);
      void run();

   private:
      std::shared_ptr<CAppWindow> createAppWindow();

   private:
      std::shared_ptr<CAppWindow> mAppWindow;
   };

//}

#endif // AERO_SIMULATOR_CAPP_H
