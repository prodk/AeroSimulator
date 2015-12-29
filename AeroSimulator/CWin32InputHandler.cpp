#include "CWin32InputHandler.h"
#include "CLog.h"
#include "CEventManager.h"
#include "../Test/CGame.h"

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
      // We do not send a genera KEDOWN_EVENT to make it easier for crossplatform development.
      // E.g. for Android we will just handle different input events, but send the same app event
      case WM_KEYDOWN:
      {
         switch (wParam)
         {
            case (0x31) : // 1 debug mode
            {
               CLog::getInstance().log("Key 1 pressed");
               CEventManager::getInstance().broadcastEvent(CGame::DEBUG_MODE_EVENT);
            }
            break;

            case (0x38) : // 8 display the depth buffer
            {
               CLog::getInstance().log("Key 8 pressed");
               CEventManager::getInstance().broadcastEvent(CGame::DEPTHBUF_EVENT);
            }
            break;
         }
      }
      return false; // WM_KEYDOWN has been processed, no need to call the default window proc
   }

   return true; // Process other messages
}
