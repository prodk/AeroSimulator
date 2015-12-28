// CWin32Window.h - a class for the Win32 window

#ifndef AERO_SIMULATOR_CWIN32WINDOW_H
#define AERO_SIMULATOR_CWIN32WINDOW_H

#include "CAppWindow.h"
#include "CWin32InputHandler.h"
#include <Windows.h>
#include <iostream>

namespace AeroSimulatorEngine
{
   class CWin32Window : public CAppWindow
   {
   public:
      CWin32Window();
      explicit CWin32Window(ePriority prio);
      virtual ~CWin32Window();

      bool create(const std::string& title, std::size_t width, std::size_t height);

      // Override CTask part
      virtual bool start() override;
      virtual void update(CTask* pTask) override;
      virtual void stop() override;

      HDC getDC() const { return mDC; }
      bool isFullScreen() const { return (mWidth == 0u || mHeight == 0u) ? true : false; }

      const CWin32InputHandler& getInputHandler() { return mInputHandler; }

   private:
      void show(bool toShow);

   private:
      HINSTANCE mInstance;
      HWND mWnd;
      HDC mDC;
      DEVMODE mScreenSettings;
      CWin32InputHandler mInputHandler;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CWIN32WINDOW_H
