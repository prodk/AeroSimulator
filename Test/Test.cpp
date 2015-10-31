// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CApp.h"

int main()
{
   CApp myApp;

   if (myApp.init("AeroSimulator", 800, 600))
   {
      myApp.run();
   }

   std::cout << std::endl << "Press any key . . . " << std::endl;
   while (!_kbhit());
   return 0;
}

