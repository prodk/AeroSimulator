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
      // We do not send a general KEYDOWN_EVENT to make it easier for crossplatform development.
      // E.g. for Android we will just handle different input events, but send the same app event
      case WM_KEYDOWN:
      {
         cameraKeyDown(wParam);
      }
      return false; // WM_KEYDOWN has been processed, no need to call the default window proc

      case WM_KEYUP:
      {
         cameraKeyUp(wParam);
      }

      return false;
   }

   return true; // Process other messages
}

void CWin32InputHandler::cameraKeyDown(WPARAM wParam) const
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
      GEventManager.broadcastEvent(eCameraEvents::DECREASE_PITCH);
      break;

   case (0x53) : // s, decrease pitch
      LOG("Key s pressed");
      GEventManager.broadcastEvent(eCameraEvents::INCREASE_PITCH);
      break;

   case (0x51) : // q, rotate camera around y-axis clock wise
      LOG("Key q pressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Y_CCW);
      break;

   case (0x45) : // e, rotate camera around y-axis counter clock wise
      LOG("Key e pressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Y_CW);
      break;

   case (0x44) : // d, rotate camera around z-axis to the right
      LOG("Key d pressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Z_CCW);
      break;

   case (0x41) : // a, rotate camera around z-axis to the left
      LOG("Key a pressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Z_CW);
      break;

   case (VK_OEM_PLUS) : // +, zoom in
      LOG("Key + pressed");
      GEventManager.broadcastEvent(eCameraEvents::ZOOM_IN);
      break;

   case (VK_OEM_MINUS) : // -, zoom in
      LOG("Key - pressed");
      GEventManager.broadcastEvent(eCameraEvents::ZOOM_OUT);
      break;
   }
}

void CWin32InputHandler::cameraKeyUp(WPARAM wParam) const
{
   switch (wParam)
   {
      // Camera
   case (0x57) : // w, increase pitch
      LOG("Key w depressed");
      GEventManager.broadcastEvent(eCameraEvents::DECREASE_PITCH_STOP);
      break;

   case (0x53) : // s, decrease pitch
      LOG("Key s depressed");
      GEventManager.broadcastEvent(eCameraEvents::INCREASE_PITCH_STOP);
      break;

   case (0x51) : // q, rotate camera display clock wise
      LOG("Key q depressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Y_CCW_STOP);
      break;

   case (0x45) : // e, rotate camera display counter clock wise
      LOG("Key e depressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Y_CCW_STOP);
      break;

   case (0x44) : // d, rotate camera around z-axis to the right
      LOG("Key d depressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Z_CCW_STOP);
      break;

   case (0x41) : // a, rotate camera around z-axis to the left
      LOG("Key a depressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Z_CW_STOP);
      break;

    case (VK_OEM_PLUS) : // +, zoom in
       LOG("Key + depressed");
       GEventManager.broadcastEvent(eCameraEvents::ZOOM_IN_STOP);
       break;

    case (VK_OEM_MINUS) : // -, zoom in
       LOG("Key - depressed");
       GEventManager.broadcastEvent(eCameraEvents::ZOOM_OUT_STOP);
       break;
   }
}
