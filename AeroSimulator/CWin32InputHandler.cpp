#include "CWin32InputHandler.h"
#include "CLog.h"
#include "CEventManager.h"
#include "CWin32Renderer.h"

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
      // We do not send a general KEDOWN_EVENT to make it easier for crossplatform development.
      // E.g. for Android we will just handle different input events, but send the same app event
      case WM_KEYDOWN:
      {
         switch (wParam)
         {
            case (0x31) : // 1 debug mode
               LOG("Key 1 pressed");
               GEventManager.broadcastEvent(DEBUG_MODE_EVENT);
               break;

            case (0x38) : // 8 display the depth buffer
               LOG("Key 8 pressed");
               GEventManager.broadcastEvent(DEPTHBUF_EVENT);
               break;

            // Camera
            case (0x57) : // w, increase pitch
               LOG("Key w pressed");
               GEventManager.broadcastEvent(eCameraEvents::INCREASE_PITCH);
               break;

            case (0x53) : // s, decrease pitch
               LOG("Key s pressed");
               GEventManager.broadcastEvent(eCameraEvents::DECREASE_PITCH);
               break;

            case (0x51) : // q, rotate camera display clock wise
               LOG("Key q pressed");
               GEventManager.broadcastEvent(eCameraEvents::ROTATE_CW);
               break;
         }
      }
      return false; // WM_KEYDOWN has been processed, no need to call the default window proc

      case WM_KEYUP:
      {
         switch (wParam)
         {
            // Camera
         case (0x57) : // w, increase pitch
            LOG("Key w pressed");
            GEventManager.broadcastEvent(eCameraEvents::INCREASE_PITCH_STOP);
            break;

         case (0x53) : // s, decrease pitch
            LOG("Key s pressed");
            GEventManager.broadcastEvent(eCameraEvents::DECREASE_PITCH_STOP);
            break;

         case (0x51) : // q, rotate camera display clock wise
            LOG("Key q pressed");
            GEventManager.broadcastEvent(eCameraEvents::ROTATE_CW_STOP);
            break;
         }
      }
         
      return false;
   }

   return true; // Process other messages
}
