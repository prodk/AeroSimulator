// CWin32Window.h - a class for the Win32 window

#ifndef AERO_SIMULATOR_CWIN32WINDOW_H
#define AERO_SIMULATOR_CWIN32WINDOW_H

#include "CAppWindow.h"
#include "CTask.h"
#include <Windows.h>
#include <iostream>

namespace AeroSimulatorEngine
{
   class CWin32Renderer;

   class CWin32Window : public CAppWindow
   {
   public:
      CWin32Window();
      explicit CWin32Window(ePriority prio);
      virtual ~CWin32Window();

      // Override CAppWindow part
      ///@todo: think how to decouple the window from the renderer. Probably use events and the task manager.
      virtual bool create(const std::string& title, std::size_t width, std::size_t height, CWin32Renderer* renderer);

      // Override CTask part
      virtual bool start();
      virtual void update(CTask* pTask);
      virtual void stop();

      HDC getDC() const { return mDC; }
      bool isFullScreen() const { return (mWidth == 0u || mHeight == 0u) ? true : false; }

   private:
      void show(bool toShow);

   private:
      HINSTANCE mInstance;
      HWND mWnd;
      HDC mDC;
      DEVMODE mScreenSettings;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CWIN32WINDOW_H
