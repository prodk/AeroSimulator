// CWin32Window.cpp - implementation of the class for the Win32 window

#include "CWin32Window.h"
#include "CEventManager.h"
#include "CApp.h"
#include "CLog.h"

#include <memory>

using namespace AeroSimulatorEngine;

// Windows procedure
LRESULT CALLBACK GlobalWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
   CWin32Window* window = NULL;
   switch (uMessage)
   {
      case WM_CREATE:
      {
         // Get the data that was saved in the CreateWindowEx as its last parameter
         LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
         window = (CWin32Window*)(lpcs->lpCreateParams);
         // place the pointer to the window "user data"
         SetWindowLong(hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)window);
      }
      break;

      case WM_CLOSE:
         ::ShowWindow(hWnd, SW_HIDE);
         PostQuitMessage(0);
         return 0;

      default:
      {
         // Extract the window pointer from the "user data"
         window = (CWin32Window*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      }
   }

   // Invoke the non-default message handler for the existing window
   if (window && !window->getInputHandler().windowProc(hWnd, uMessage, wParam, lParam))
   {
      return 0; // The message has been processed by the non-default win proc
   }

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

bool CWin32Window::create(const std::string& title, std::size_t width, std::size_t height)
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
      this)))
   {
      CLog::getInstance().log("* window was created");
   }
   else
   {
      CLog::getInstance().log("* Error: window wasn't created");
      return false;
   }

   /// Do not forget to cleanup the wide character string.
   delete pwcTitle;

   if ((mDC = GetDC(mWnd)))
   {
      CLog::getInstance().log("* device context for window was obtained");
   }
   else
   {
      CLog::getInstance().log("* Error: device context for window wasn't obtained");
      return false;
   }

   return true;
}

void CWin32Window::show(bool toShow)
{
   // Set the highest priority
   ::SetForegroundWindow(mWnd);
   if (toShow)
   {
      ::ShowWindow(mWnd, SW_SHOW);
   }
   else
   {
      ::ShowWindow(mWnd, SW_HIDE);
   }
}

bool CWin32Window::start()
{
   show(true);

   return true;
}

void CWin32Window::update(CTask* pTask)
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
         {
            CLog::getInstance().log("* device context was destroyed");
         }
         else
         {
            CLog::getInstance().log("* ERROR: device context wasn't destroyed");
         }
         mDC = NULL;
      }

      if (DestroyWindow(mWnd))
      {
         CLog::getInstance().log("* window was destroyed");
      }
      else
      {
         CLog::getInstance().log("* ERROR: window wasn't destroyed");
      }
      mWnd = NULL;

      UnregisterClass(L"AERO_SIMULATOR_WINDOW", mInstance);
   }

   // Back to the mode that existed before the app started
   if (isFullScreen())
   {
      ChangeDisplaySettings(NULL, 0);
   }
}

