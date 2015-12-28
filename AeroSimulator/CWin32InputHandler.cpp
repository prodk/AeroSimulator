#include "CWin32InputHandler.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

CWin32InputHandler::CWin32InputHandler()
{
}

CWin32InputHandler::~CWin32InputHandler()
{
}

bool CWin32InputHandler::windowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) const
{
   switch (uMessage)
   {
   case WM_KEYDOWN:
   {
      switch (wParam)
      {
         case (0x38) : // 8 display the depth buffer
         {
            CLog::getInstance().log("Key 8 pressed!");
         }
         break;
      }
   }
   return false; // WM_KEYDOWN has been processed, no need to call the default window proc
   }

   return true; // Process other messages
}
