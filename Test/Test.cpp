// Test.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "CApp.h"
#include "CGame.h"
using namespace AeroSimulatorEngine;

int main()
{
   CGame gameTask(CTask::HIGH_PRIO); // game is updated before the renderer
   ///@todo: add non-default CApp constructor which initializes the app from some state.
   if (CApp::getInstance().init("AeroSimulator", 1280, 720))
   {
      //CApp::getInstance().addTask(&gameTask);
      CApp::getInstance().run();
   }

   return CApp::getInstance().exit();

   ///@todo: fix the app cleanup when the Test.exe shuts down. Currently we get the error code 0xc000013a or smth like that. Should be 0.
}

