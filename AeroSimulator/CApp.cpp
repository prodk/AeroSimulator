// CApp.cpp

#include "CApp.h"
#include "CWin32Window.h"
using namespace AeroSimulatorEngine;

CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(createAppWindow())
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
   }

   mTaskManager.addTask(mAppWindowTask.get());

   return result;
}

void CApp::run()
{
   mTaskManager.execute();
}

std::shared_ptr<CAppWindow> CApp::createAppWindow()
{
   ///@todo: check for the platform here and return the proper window type.
   ///@todo: i.e. for Windows return CWin32Window, for Android - CAndroidWindow.
   return std::shared_ptr<CAppWindow>(new CWin32Window(CTask::HIGHEST_PRIO));
}
