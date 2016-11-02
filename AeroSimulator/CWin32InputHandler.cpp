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
      handleKeyDown(wParam);
   }
   return false; // WM_KEYDOWN has been processed, no need to call the default window proc

   case WM_KEYUP:
   {
      handleKeyUp(wParam);
   }

   return false;
   }

   return true; // Process other messages
}

void CWin32InputHandler::handleKeyDown(WPARAM wParam) const
{
   switch (wParam)
   {
   case (0x31): // 1 debug mode
      //LOG("Key 1 pressed");
      GEventManager.broadcastEvent(DEBUG_MODE_EVENT);
      break;

   case (0x38): // 8 display the depth buffer
      //LOG("Key 8 pressed");
      GEventManager.broadcastEvent(DEPTHBUF_EVENT);
      break;

      // Camera
   case (0x57): // w, increase pitch
      //LOG("Key w pressed");
      GEventManager.broadcastEvent(eCameraEvents::INCREASE_PITCH);
      break;

   case (0x53): // s, decrease pitch
      //LOG("Key s pressed");
      GEventManager.broadcastEvent(eCameraEvents::DECREASE_PITCH);
      break;

   case (0x51): // q, rotate camera around y-axis clock wise
      //LOG("Key q pressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Z_CW);
      break;

   case (0x45): // e, rotate camera around y-axis counter clock wise
      //LOG("Key e pressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Z_CCW);
      break;

   case (0x44): // d, rotate camera around z-axis to the right
      //LOG("Key d pressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Y_CCW);
      break;

   case (0x41): // a, rotate camera around z-axis to the left
      //LOG("Key a pressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Y_CW);
      break;

   case (VK_OEM_PLUS): // +, zoom in
      //LOG("Key + pressed");
      GEventManager.broadcastEvent(eCameraEvents::ZOOM_IN);
      break;

   case (VK_OEM_MINUS): // -, zoom in
      //LOG("Key - pressed");
      GEventManager.broadcastEvent(eCameraEvents::ZOOM_OUT);
      break;

   case (0x5A): // z, move left
      //LOG("Key z pressed");
      GEventManager.broadcastEvent(eCameraEvents::MOVE_LEFT);
      break;

   case (0x58): // x, move right
      //LOG("Key x pressed");
      GEventManager.broadcastEvent(eCameraEvents::MOVE_RIGHT);
      break;

   case (0x46): // f, move up
      //LOG("Key f pressed");
      GEventManager.broadcastEvent(eCameraEvents::MOVE_UP);
      break;

   case (0x56): // v, move down
      //LOG("Key v pressed");
      GEventManager.broadcastEvent(eCameraEvents::MOVE_DOWN);
      break;

   case (VK_LEFT): // left, lean the airplane to the left
      LOG("Key arrow left pressed");
      GEventManager.broadcastEvent(eAirplaneEvents::LEAN_LEFT_START);
      break;

   case (VK_RIGHT): // right, lean the airplane to the right
      LOG("Key arrow right pressed");
      GEventManager.broadcastEvent(eAirplaneEvents::LEAN_RIGHT_START);
      break;

   case (VK_UP): // up, increase the height
      LOG("Key arrow up pressed");
      GEventManager.broadcastEvent(eAirplaneEvents::GO_UP_START);
      break;

   case (VK_DOWN): // down, decrease the height
      LOG("Key arrow down pressed");
      GEventManager.broadcastEvent(eAirplaneEvents::GO_DOWN_START);
      break;
   }
}

void CWin32InputHandler::handleKeyUp(WPARAM wParam) const
{
   switch (wParam)
   {
      // Camera
   case (0x57): // w, increase pitch
      //LOG("Key w depressed");
      GEventManager.broadcastEvent(eCameraEvents::INCREASE_PITCH_STOP);
      break;

   case (0x53): // s, decrease pitch
      //LOG("Key s depressed");
      GEventManager.broadcastEvent(eCameraEvents::DECREASE_PITCH_STOP);
      break;

   case (0x51): // q, rotate camera display clock wise
      //LOG("Key q depressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Z_CW_STOP);
      break;

   case (0x45): // e, rotate camera display counter clock wise
      //LOG("Key e depressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Z_CCW_STOP);
      break;

   case (0x44): // d, rotate camera around z-axis to the right
      //LOG("Key d depressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Y_CCW_STOP);
      break;

   case (0x41): // a, rotate camera around z-axis to the left
      //LOG("Key a depressed");
      GEventManager.broadcastEvent(eCameraEvents::ROTATE_Y_CCW_STOP);
      break;

   case (VK_OEM_PLUS): // +, zoom in
      //LOG("Key + depressed");
      GEventManager.broadcastEvent(eCameraEvents::ZOOM_IN_STOP);
      break;

   case (VK_OEM_MINUS): // -, zoom in
      //LOG("Key - depressed");
      GEventManager.broadcastEvent(eCameraEvents::ZOOM_OUT_STOP);
      break;

   case (0x5A): // z, move left
      //LOG("Key z depressed");
      GEventManager.broadcastEvent(eCameraEvents::MOVE_LEFT_STOP);
      break;

   case (0x58): // x, move right
      //LOG("Key x depressed");
      GEventManager.broadcastEvent(eCameraEvents::MOVE_RIGHT_STOP);
      break;

   case (0x46): // f, move up
      //LOG("Key f depressed");
      GEventManager.broadcastEvent(eCameraEvents::MOVE_UP_STOP);
      break;

   case (0x56): // v, move down
      //LOG("Key v depressed");
      GEventManager.broadcastEvent(eCameraEvents::MOVE_DOWN_STOP);
      break;

   case (VK_LEFT): // left, lean the airplane to the left
      LOG("Key arrow left depressed");
      GEventManager.broadcastEvent(eAirplaneEvents::LEAN_LEFT_STOP);
      break;

   case (VK_RIGHT): // right, lean the airplane to the right
      LOG("Key arrow right depressed");
      GEventManager.broadcastEvent(eAirplaneEvents::LEAN_RIGHT_STOP);
      break;

   case (VK_UP): // up, increase the height
      LOG("Key arrow up depressed");
      GEventManager.broadcastEvent(eAirplaneEvents::GO_UP_STOP);
      break;

   case (VK_DOWN): // down, decrease the height
      LOG("Key arrow down depressed");
      GEventManager.broadcastEvent(eAirplaneEvents::GO_DOWN_STOP);
      break;
   }
}
