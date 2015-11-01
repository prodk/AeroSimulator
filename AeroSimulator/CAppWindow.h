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
      virtual ~CAppWindow();

      static bool isClosing() { return mIsClosing; }
      static void resetIsClosing() { mIsClosing = false; }

      virtual bool create(const std::string& title, std::size_t width, std::size_t height) = 0;

   protected:
      std::string mTitle;
      std::size_t mWidth;
      std::size_t mHeight;

      static bool mIsClosing;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPPWINDOW_H
