// CWin32Window.cpp - implementation of the class for the Win32 window

#include "CWin32Window.h"
#include "CLog.h"
#include "CWin32Renderer.h"
#include <memory>

using namespace AeroSimulatorEngine;

// Windows procedure
LRESULT CALLBACK GlobalWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
   //CWin32Window* window = NULL;
   CWin32Renderer* renderer = NULL;
   switch (uMessage)
   {
   case WM_CREATE:
   {
      // Get the data that was saved in the CreateWindowEx as its last parameter
      LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
      //window = (CWin32Window*)(lpcs->lpCreateParams);
      renderer = (CWin32Renderer*)(lpcs->lpCreateParams);
      // place the pointer to the window "user data"
      //SetWindowLong(hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)window);
      SetWindowLong(hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)renderer);
   }
   break;

   case WM_CLOSE:
      ::ShowWindow(hWnd, SW_HIDE);
      PostQuitMessage(0);
      return 0;
   default:
   {
      // Extract the window pointer from the "user data"
      //window = (CWin32Window*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      renderer = (CWin32Renderer*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
   }
   }

   ///@todo: uncomment when relevant (i.e. the UI input is processed): invoke the message handler for the existing window
   //if (window && !window->windowProc(uMessage, wParam, lParam))
   if (renderer && !renderer->windowProc(uMessage, wParam, lParam))
   return 0;

   // Invoke the standard message handler for unprocessed messages
   return ::DefWindowProc(hWnd, uMessage, wParam, lParam);
}

CWin32Window::CWin32Window()
   : CAppWindow()
   , mInstance(0)
   , mWnd(0)
   , mDC(0)
   , mScreenSettings()
{
}

CWin32Window::CWin32Window(ePriority prio)
   : CAppWindow(prio)
   , mInstance(0)
   , mWnd(0)
   , mDC(0)
   , mScreenSettings()
{
}

CWin32Window::~CWin32Window()
{
   stop();
}

bool CWin32Window::create(const std::string& title, std::size_t width, std::size_t height, CWin32Renderer* renderer)
{
   mInstance = GetModuleHandle(NULL);
   mWidth = width;
   mHeight = height;
   HICON hIcon = LoadIcon(NULL, IDI_APPLICATION);

   // Register the window class
   WNDCLASSEX winClass;
   winClass.lpszClassName = L"AERO_SIMULATOR_WINDOW";
   winClass.cbSize = sizeof(WNDCLASSEX);
   winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   winClass.lpfnWndProc = (WNDPROC)GlobalWndProc;
   winClass.cbClsExtra = 0;
   winClass.cbWndExtra = sizeof(LONG_PTR);
   winClass.hInstance = mInstance;
   winClass.hIcon = hIcon;
   winClass.hIconSm = hIcon;
   winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
   winClass.hbrBackground = NULL;
   winClass.lpszMenuName = NULL;
   RegisterClassEx(&winClass);

   // Window styles
   DWORD dwExStyle;
   DWORD dwStyle;

   if (!isFullScreen())
   {
      dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      dwStyle = WS_OVERLAPPEDWINDOW;
      dwStyle &= ~WS_MAXIMIZEBOX;
      dwStyle &= ~WS_THICKFRAME;
   }
   else
   {
      // Full screen settings
      memset(&mScreenSettings, 0, sizeof(DEVMODE));
      mScreenSettings.dmSize = sizeof(DEVMODE);
      mScreenSettings.dmPelsWidth = width;
      mScreenSettings.dmPelsHeight = height;
      mScreenSettings.dmBitsPerPel = 32;
      mScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

      // Set window styles
      dwExStyle = WS_EX_APPWINDOW;
      dwStyle = WS_POPUP;
   }

   // Set the client part of the window (taking into account the window styles)
   RECT rectWindow;
   rectWindow.left = (long)0;
   rectWindow.right = (long)width;
   rectWindow.top = (long)0;
   rectWindow.bottom = (long)height;

   AdjustWindowRectEx(&rectWindow, dwStyle, FALSE, dwExStyle);

   //@todo: check if UNICODE
   // Convert to wide characters.
   int nChars = MultiByteToWideChar(CP_ACP, 0, title.c_str(), -1, NULL, 0);
   wchar_t* pwcTitle = NULL;
   MultiByteToWideChar(CP_ACP, 0, title.c_str(), -1, (LPWSTR)pwcTitle, nChars);

   // Window creation
   if ((mWnd = CreateWindowEx(dwExStyle,
      winClass.lpszClassName,
      pwcTitle,
      dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      0, 0,
      rectWindow.right - rectWindow.left,
      rectWindow.bottom - rectWindow.top,
      NULL,
      NULL,
      mInstance,
      renderer)))
      //this)))
   {
      std::cout << "* window was created" << std::endl;
   }
   else
   {
      std::cout << "* Error: window wasn't created" << std::endl;
      return false;
   }

   /// Do not forget to cleanup the wide character string.
   delete pwcTitle;

   if ((mDC = GetDC(mWnd)))
   {
      std::cout << "* device context for window was obtained" << std::endl;
   }
   else
   {
      std::cout << "* Error: device context for window wasn't obtained" << std::endl;
      return false;
   }

   return true;
}

///@todo: remove this method
//bool CWin32Window::windowProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
//{
//   switch (uMessage)
//   {
//      // Stubs for system commands
//   case WM_SYSCOMMAND:
//   {
//      switch (wParam)
//      {
//         // Screensaver is trying to start
//      case SC_SCREENSAVE:
//         // The display is trying to switch off
//      case SC_MONITORPOWER:
//         return false;
//      }
//      break;
//   }
//   return false;
//
//   // System keystrokes stubs
//   case WM_SYSKEYDOWN:
//   case WM_SYSKEYUP:
//   {}
//   return false;
//
//   // Finishing
//   case WM_CLOSE:
//   case WM_QUIT:
//   {
//      // Exit flag
//      mIsClosing = true;
//   }
//   return false;
//
//   // Keyboard is pressed
//   case WM_KEYDOWN:
//   {
//      if (wParam == VK_LEFT)
//      {
//         int pressed = 1;
//         CLog::getInstance().log("Left pressed");
//      }
//      if (wParam == VK_RIGHT)
//      {
//         int pressed = 0;
//         CLog::getInstance().log("Right pressed");
//      }
//      if (wParam == VK_UP)
//      {
//         int pressed = 1;
//         CLog::getInstance().log("Up pressed");
//      }
//      if (wParam == VK_DOWN)
//      {
//         int pressed = 0;
//         CLog::getInstance().log("Down pressed");
//      }
//   }
//   return false;
//
//   // Keyboard is depressed
//   case WM_KEYUP:
//   {
//      if (wParam == VK_LEFT)
//      {
//         int pressed = 1;
//         CLog::getInstance().log("Left DEpressed");
//      }
//      if (wParam == VK_RIGHT)
//      {
//         int pressed = 0;
//         CLog::getInstance().log("Right DEpressed");
//      }
//      if (wParam == VK_UP)
//      {
//         int pressed = 1;
//         CLog::getInstance().log("Up DEpressed");
//      }
//      if (wParam == VK_DOWN)
//      {
//         int pressed = 0;
//         CLog::getInstance().log("Down DEpressed");
//      }
//   }
//   return false;
//   } // end switch
//
//   // Process other messages
//   return true;
//}

void CWin32Window::show(bool toShow)
{
   // Set the highest priority
   ::SetForegroundWindow(mWnd);
   if (toShow)
      ::ShowWindow(mWnd, SW_SHOW);
   else
      ::ShowWindow(mWnd, SW_HIDE);
}

bool CWin32Window::start()
{
   show(true);

   return true;
}

void CWin32Window::update()
{
   MSG msg;
   ZeroMemory(&msg, sizeof(MSG));

   if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
   {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
   }

   if ((WM_QUIT == msg.message) || (WM_CLOSE == msg.message))
   {
      mIsClosing = true;
   }
}

void CWin32Window::stop()
{
   if (mWnd)
   {
      if (mDC)
      {
         if (ReleaseDC(mWnd, mDC))
            std::cout << "* device context was destroyed" << std::endl;
         else
            std::cout << "* ERROR: device context wasn't destroyed" << std::endl;
         mDC = NULL;
      }

      if (DestroyWindow(mWnd))
         std::cout << "* window was destroyed" << std::endl;
      else
         std::cout << "* ERROR: window wasn't destroyed" << std::endl;
      mWnd = NULL;

      UnregisterClass(L"AERO_SIMULATOR_WINDOW", mInstance);
   }

   // Back to the mode that existed before the app started
   if (isFullScreen())
   {
      ChangeDisplaySettings(NULL, 0);
   }
}

