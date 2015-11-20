// Test.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "CApp.h"
using namespace AeroSimulatorEngine;

int main()
{
   ///@todo: add non-default CApp constructor which initializes the app from some state.

   if (CApp::getInstance().init("AeroSimulator", 1024, 640))
   {
      CApp::getInstance().run();
   }

   return CApp::getInstance().exit();

   ///@todo: fix the app cleanup when the Test.exe shuts down. Currently we get the error code 0xc000013a or smth like that. Should be 0.
}

