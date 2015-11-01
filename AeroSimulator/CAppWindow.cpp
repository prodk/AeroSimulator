#include "CAppWindow.h"

bool CAppWindow::mIsClosing = false;

CAppWindow::CAppWindow()
   : mTitle()
   , mWidth(0u)
   , mHeight(0u)
{
}

CAppWindow::~CAppWindow()
{
}
