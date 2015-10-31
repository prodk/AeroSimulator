#include "CWin32Window.h"
#include <memory>

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

   ///@todo: uncomment when relevant (i.e. the UI input is processed): invoke the message handler for the existing window
   //if (window && !window->WindowProc(uMessage, wParam, lParam))
   //return 0;

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

CWin32Window::~CWin32Window()
{
   if (mWnd)
   {
      if (mDC)
      {
         if (ReleaseDC(mWnd, mDC))
            //Log::instance() << "* device context was destroyed" << endl;
            std::cout << "* device context was destroyed" << std::endl;
         else
            //Log::instance() << "* ERROR: device context wasn't destroyed" << endl;
            std::cout << "* ERROR: device context wasn't destroyed" << std::endl;
         mDC = NULL;
      }

      if (DestroyWindow(mWnd))
         //Log::instance() << "* window was destroyed" << endl;
         std::cout << "* window was destroyed" << std::endl;
      else
         //Log::instance() << "* ERROR: window wasn't destroyed" << endl;
         std::cout << "* ERROR: window wasn't destroyed" << std::endl;
      mWnd = NULL;

      // освобождаем класс окна
      UnregisterClass(L"AERO_SIMULATOR_WINDOW", mInstance);
   }

   // возвращаемся в режим до запуска приложения
   if (isFullscreen())
   {
      ChangeDisplaySettings(NULL, 0);
   }
}

bool CWin32Window::create(const std::string& title, std::size_t width, std::size_t height)
{
   mInstance = GetModuleHandle(NULL);
   mWidth = width;
   mHeight = height;
   HICON hIcon = LoadIcon(NULL, IDI_APPLICATION);

   // регистрируем класс окна
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

   // стили отображения окна
   DWORD dwExStyle;
   DWORD dwStyle;

   if (!isFullscreen())
   {
      // задаем стили отображения окна
      dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      dwStyle = WS_OVERLAPPEDWINDOW;
      dwStyle &= ~WS_MAXIMIZEBOX;
      dwStyle &= ~WS_THICKFRAME;
   }
   else
   {
      // настройки режима для полноэкранного приложения
      memset(&mScreenSettings, 0, sizeof(DEVMODE));
      mScreenSettings.dmSize = sizeof(DEVMODE);
      mScreenSettings.dmPelsWidth = width;
      mScreenSettings.dmPelsHeight = height;
      mScreenSettings.dmBitsPerPel = 32;

      //@todo: remove this one
     /* if (!GetMaxFrequency(m_dmScreenSettings))
      {
         Log::instance() << "* Error: screen resolution isn't supported (" << width << "x" << height << "x32b)" << endl;
         return false;
      }
      else
      {
         Log::instance() << "* Screen resolution is supported: " << width << "x" << height << "x32b@" << m_dmScreenSettings.dmDisplayFrequency << "Hz" << endl;
      }*/

      mScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

      // задаем стили отображения окна
      dwExStyle = WS_EX_APPWINDOW;
      dwStyle = WS_POPUP;
   }

   // задаем клиентскую часть окна (с учетом стилей отображения)
   RECT rectWindow;
   rectWindow.left = (long)0;
   rectWindow.right = (long)width;
   rectWindow.top = (long)0;
   rectWindow.bottom = (long)height;

   // пересчитываем размеры окна с учетом указаных стилей отображения
   AdjustWindowRectEx(&rectWindow, dwStyle, FALSE, dwExStyle);

   // Convert to wide characters.
   int nChars = MultiByteToWideChar(CP_ACP, 0, title.c_str(), -1, NULL, 0);
   wchar_t* pwcTitle = NULL;
   // required size

   MultiByteToWideChar(CP_ACP, 0, title.c_str(), -1, (LPWSTR)pwcTitle, nChars);

   // создание окна
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
      //Log::instance() << "* window was created" << endl;
      std::cout << "* window was created" << std::endl;
   }
   else
   {
      //Log::instance() << "* Error: window wasn't created" << endl;
      std::cout << "* Error: window wasn't created" << std::endl;
      return false;
   }

   delete pwcTitle;

   // получаем контекст устройства для окна
   if ((mDC = GetDC(mWnd)))
   {
      //Log::instance() << "* device context for window was got" << endl;
      std::cout << "* device context for window was obtained" << std::endl;
   }
   else
   {
      //Log::instance() << "* Error: device context for window wasn't got" << endl;
      std::cout << "* Error: device context for window wasn't obtained" << std::endl;
      return false;
   }

   return true;
}

void CWin32Window::show()
{
   // Set the highest priority
   ::SetForegroundWindow(mWnd);
   ::ShowWindow(mWnd, SW_SHOW);
}

void CWin32Window::run()
{
   MSG msg;
   ZeroMemory(&msg, sizeof(MSG));

   while ((WM_QUIT != msg.message) && (WM_CLOSE != msg.message)) // TODO: check the event handler here when it is available.
   {
      //if (::PeekMessage(&msg, mWnd, 0, 0, PM_REMOVE))
      if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
      {
         ::TranslateMessage(&msg);
         ::DispatchMessage(&msg);
      }
      else
      {

      }
   }
}

