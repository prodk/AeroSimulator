// CApp.cpp

#include "CApp.h"
#include "CWin32Window.h"

CApp::CApp()
   : mAppWindow(createAppWindow())
{
   std::cout << "CApp created!" << std::endl;
}

CApp::~CApp()
{
}

bool CApp::init()
{
   return true;
}

void CApp::run()
{
}

std::shared_ptr<CAppWindow> CApp::createAppWindow()
{
   ///@todo: check for the platform here and return the proper window type.
   return std::shared_ptr<CAppWindow>(new CWin32Window());
}
