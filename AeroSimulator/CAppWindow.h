//

//#pragma once
#ifndef AERO_SIMULATOR_CAPPWINDOW_H
#define AERO_SIMULATOR_CAPPWINDOW_H

class CAppWindow
{
public:
   CAppWindow();
   virtual ~CAppWindow();

   virtual bool create() = 0;
};

#endif // AERO_SIMULATOR_CAPPWINDOW_H
