// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CApp.h"

int main()
{
   ///@todo: add non-default CApp constructor which initializes the app from some state.

   if (CApp::getInstance().init("AeroSimulator", 800, 600))
   {
      CApp::getInstance().run();
   }

   // TODO: destroy the AppWindow and show the messages before the "Press any key" message.

   std::cout << std::endl << "Press any key . . . " << std::endl;
   while (!_kbhit());
   return 0;

   // TODO: fix the app cleanup when the Test.exe shuts down. Currently we get the error code 0xc000013a or smth like that. Should be 0.
}

