//

//#pragma once
#ifndef AERO_SIMULATOR_CAPPWINDOW_H
#define AERO_SIMULATOR_CAPPWINDOW_H
#include <string>

class CAppWindow
{
public:
   CAppWindow();
   virtual ~CAppWindow();

   virtual bool create(const std::string& title, std::size_t width, std::size_t height) = 0;
   virtual void show() = 0;
   virtual void run() = 0; // TODO: remove this when CWin32 is inherited from the task.

protected:
   std::string mTitle;
   std::size_t mWidth;
   std::size_t mHeight;
};

#endif // AERO_SIMULATOR_CAPPWINDOW_H
