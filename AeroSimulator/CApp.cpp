// CApp.cpp

#include "CApp.h"
#include "CWin32Window.h"
#include <conio.h>
using namespace AeroSimulatorEngine;

CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(new CWin32Window(CTask::HIGHEST_PRIO))
   , mRendererTask(new CWin32Renderer(CTask::HIGH_PRIO))
{
   std::cout << "CApp created!" << std::endl;
}

CApp::~CApp()
{
   mAppWindowTask = nullptr;
   std::cout << "CApp destroyed" << std::endl;
}

bool CApp::init(const char* name, unsigned int width, unsigned int height)
{
   bool result = false;
   if (mAppWindowTask && mAppWindowTask->create(name, width, height))
   {
      result = true;

      mRendererTask->init(*mAppWindowTask);
   }

   mTaskManager.addTask(mAppWindowTask.get());
   mTaskManager.addTask(mRendererTask.get());

   return result;
}

void CApp::run()
{
   mTaskManager.execute();
}

int CApp::exit()
{
   mAppWindowTask = nullptr;
   mRendererTask = nullptr;

   std::cout << std::endl << "Press any key . . . " << std::endl;
   while (!_kbhit());
   return 0;
}
