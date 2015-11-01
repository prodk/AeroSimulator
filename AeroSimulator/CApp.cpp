// CApp.cpp

#include "CApp.h"
#include "CWin32Window.h"

// TODO: make CApp a singleton
CApp::CApp()
   : mAppWindow(createAppWindow())
{
   std::cout << "CApp created!" << std::endl;
}

CApp::~CApp()
{
   mAppWindow = 0;
   std::cout << "CApp destroyed" << std::endl;
}

bool CApp::init(const char* name, unsigned int width, unsigned int height)
{
   bool result = false;
   if (mAppWindow && mAppWindow->create(name, width, height))
   {
      result = true;
   }

   return result;
}

void CApp::run()
{
   mAppWindow->show();

   mAppWindow->run();

   ///@todo temporary
   mAppWindow = 0;
}

std::shared_ptr<CAppWindow> CApp::createAppWindow()
{
   ///@todo: check for the platform here and return the proper window type.
   return std::shared_ptr<CAppWindow>(new CWin32Window());
}
