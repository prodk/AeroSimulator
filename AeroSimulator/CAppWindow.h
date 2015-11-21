// CAppWindow.h - declaration of the abstract class for a window on all platforms

#ifndef AERO_SIMULATOR_CAPPWINDOW_H
#define AERO_SIMULATOR_CAPPWINDOW_H
#include "CTask.h"
#include <string>

namespace AeroSimulatorEngine
{
   // Every window IS A CTask
   class CAppWindow : public CTask
   {
   public:
      CAppWindow();
      explicit CAppWindow(ePriority prio);
      virtual ~CAppWindow() = 0;

      static bool isClosing() { return mIsClosing; }
      static void resetIsClosing() { mIsClosing = false; }

   protected:
      std::string mTitle;
      std::size_t mWidth;
      std::size_t mHeight;

      static bool mIsClosing;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPPWINDOW_H
