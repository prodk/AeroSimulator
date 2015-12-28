#ifndef AERO_SIMULATOR_CWIN32_INPUT_HANDLER_H
#define AERO_SIMULATOR_CWIN32_INPUT_HANDLER_H

#include <Windows.h>

namespace AeroSimulatorEngine
{
   // This class decouples the window from unnecessary knowledge of 
   // the mapping between keystrokes and corresponding game events
   class CWin32InputHandler
   {
   public:
      CWin32InputHandler();
      ~CWin32InputHandler();

      bool windowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) const;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CWIN32_INPUT_HANDLER_H
