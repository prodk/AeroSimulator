//#pragma once

#ifndef AERO_SIMULATOR_CWIN32WINDOW_H
#define AERO_SIMULATOR_CWIN32WINDOW_H

#include "CAppWindow.h"
#include <Windows.h>
#include <iostream>

class CWin32Window : public CAppWindow
{
public:
   CWin32Window();
   virtual ~CWin32Window();

   virtual bool create(const std::string& title, std::size_t width, std::size_t height);
   virtual void show();
   virtual void run();

   HDC getDC() const { return mDC; }

private:
   bool isFullscreen() { return (mWidth == 0u || mHeight == 0u) ? true : false; }

private:
   HINSTANCE mInstance;
   HWND mWnd;
   HDC mDC;
   DEVMODE mScreenSettings;
};

#endif // AERO_SIMULATOR_CWIN32WINDOW_H
