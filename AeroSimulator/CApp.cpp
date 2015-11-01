// CApp.cpp

#include "CApp.h"
#include "CWin32Window.h"

// TODO: make CApp a singleton
CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(createAppWindow())
{
   std::cout << "CApp created!" << std::endl;
}

CApp::~CApp()
{
   mAppWindowTask = 0;
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
   // TODO: this part works
   /*mAppWindowTask->show(true);
   mAppWindowTask->run();
   ///@todo temporary
   mAppWindowTask = 0;*/
   // TODO: end
   mTaskManager.execute();
}

std::shared_ptr<CAppWindow> CApp::createAppWindow()
{
   ///@todo: check for the platform here and return the proper window type.
   return std::shared_ptr<CAppWindow>(new CWin32Window(CTask::HIGHEST_PRIO));
}
