//#pragma once

#ifndef AERO_SIMULATOR_CWIN32WINDOW_H
#define AERO_SIMULATOR_CWIN32WINDOW_H

#include "CAppWindow.h"

class CWin32Window : public CAppWindow
{
public:
   CWin32Window();
   virtual ~CWin32Window();

   virtual bool create();
};

#endif // AERO_SIMULATOR_CWIN32WINDOW_H
