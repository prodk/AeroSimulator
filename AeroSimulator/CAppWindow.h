//

//#pragma once
#ifndef AERO_SIMULATOR_CAPPWINDOW_H
#define AERO_SIMULATOR_CAPPWINDOW_H
#include <string>

#include "CTask.h"

class CAppWindow : public CTask
{
public:
   CAppWindow();
   explicit CAppWindow(ePriority prio);
   virtual ~CAppWindow();

   virtual bool create(const std::string& title, std::size_t width, std::size_t height) = 0;
   virtual void show(bool toShow) = 0;
   virtual void run() = 0; // TODO: remove this when CWin32 is inherited from the task.

   static bool isClosing() { return mIsClosing; }
   static void resetIsClosing() { mIsClosing = false; }

protected:
   std::string mTitle;
   std::size_t mWidth;
   std::size_t mHeight;

   static bool mIsClosing;
};

#endif // AERO_SIMULATOR_CAPPWINDOW_H
