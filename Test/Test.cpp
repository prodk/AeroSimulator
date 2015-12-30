// Test.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "CApp.h"
#include "CGame.h"
#include "CLog.h"

#include <Windows.h>
using namespace AeroSimulatorEngine;

int main()
{
   CGame gameTask(CTask::HIGH_PRIO); // game is updated before the renderer

   ///@todo: add non-default CApp constructor which initializes the app from some state.
   ///@todo: add a constructor that reads the ini file, the state is also specified in the ini
   if (CApp::getInstance().init("AeroSimulator", 1280, 720))
   {
      CApp::getInstance().addTask(&gameTask);
      CApp::getInstance().run();
   }

   return CApp::getInstance().exit();
}

