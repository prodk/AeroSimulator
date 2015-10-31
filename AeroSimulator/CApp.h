//#pragma once

#ifndef AERO_SIMULATOR_CAPP_H
#define AERO_SIMULATOR_CAPP_H

#include <iostream>
#include <memory>
#include "CAppWindow.h"

//namespace AeroSimulatorEngine
//{
   class CApp
   {
   public:
      CApp();
      ~CApp();

      bool init();
      void run();

   private:
      std::shared_ptr<CAppWindow> createAppWindow();

   private:
      std::shared_ptr<CAppWindow> mAppWindow;
   };

//}

#endif // AERO_SIMULATOR_CAPP_H
