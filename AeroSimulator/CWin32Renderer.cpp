#include "CWin32Renderer.h"
using namespace AeroSimulatorEngine;

CWin32Renderer::CWin32Renderer(ePriority prio, const CWin32Window& window)
   : CRenderer(prio)
   , mDC(window.getDC())
   , mIsInitialized(false)
   , mRenderContext(0)
   , mOldRenderContext(0)
   , mIsFullScreen(window.isFullScreen())
{
   init();
}

CWin32Renderer::~CWin32Renderer()
{
   destroy();
}

bool AeroSimulatorEngine::CWin32Renderer::start()
{
   return isInitialized();
}

bool AeroSimulatorEngine::CWin32Renderer::update()
{
   return false;
}

bool AeroSimulatorEngine::CWin32Renderer::stop()
{
   return false;
}

void AeroSimulatorEngine::CWin32Renderer::init()
{
   if (createRenderContext())
   {
      if (setRenderContext() && loadOpenGLExtensions())
      {
         mIsInitialized = true;
         // Go back to the window rendering context
         resetRenderContext();
      }
   }
}

void AeroSimulatorEngine::CWin32Renderer::destroy()
{
   /*wglMakeCurrent(NULL, NULL);

   if (m_bOwnerRC && m_hRC)
   {
      if (wglDeleteContext(m_hRC))
         Log::instance() << "* render context was destroyed" << endl;
      else
         Log::instance() << "* ERROR: render context wasn't destroyed" << endl;
      m_hRC = NULL;
   }*/
}

void AeroSimulatorEngine::CWin32Renderer::draw()
{
}

void AeroSimulatorEngine::CWin32Renderer::swapBuffers()
{
   ::SwapBuffers(mDC);
}

bool AeroSimulatorEngine::CWin32Renderer::createRenderContext()
{
   bool result = false;

   // Choose the pixel format
   PIXELFORMATDESCRIPTOR pfd;
   memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   pfd.nVersion = 1;
   if (mIsFullScreen)
      pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED;
   else
      pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 32;
   pfd.cAlphaBits = 8;
   pfd.cDepthBits = 0;
   pfd.cStencilBits = 0;
   pfd.iLayerType = PFD_MAIN_PLANE;
   UINT uiPixelFormat = ::ChoosePixelFormat(mDC, &pfd);
   if (uiPixelFormat)
   {
      std::cout << "* pixel format was found: " << uiPixelFormat << std::endl;
   }
   else
   {
      std::cout << "* Error: pixel format wasn't found" << std::endl;
      return false;
   }

   // устанавливаем формат пиксела
   if (::SetPixelFormat(mDC, uiPixelFormat, &pfd))
   {
      std::cout << "* pixel format was set" << std::endl;
   }
   else
   {
      std::cout << "* Error: pixel format wasn't set" << std::endl;
      return false;
   }

   // Init GLEW
   HGLRC tempContext = wglCreateContext(mDC);

   // TODO: uncomment when using GLEW and not a manual exteinsion loading
   //wglMakeCurrent(m_hDC, tempContext);

   //GLenum err = glewInit();
   //if (GLEW_OK != err)
   //{
   //   Log::instance() << "GLEW is not initialized!" << endl;
   //}

   //int attribs[] =
   //{
   //   WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
   //   WGL_CONTEXT_MINOR_VERSION_ARB, 1,
   //   WGL_CONTEXT_FLAGS_ARB, 0,
   //   0
   //};

   //if (wglewIsSupported("WGL_ARB_create_context") == 1)
   //{
   //   m_hRC = wglCreateContextAttribsARB(m_hDC, 0, attribs);
   //   wglMakeCurrent(NULL, NULL);
   //   wglDeleteContext(tempContext);
   //   //wglMakeCurrent(m_hDC, m_hRC); // Making the context current is done in setRC
   //}
   //else
   {  //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
      mRenderContext = tempContext;
   }

   if (mRenderContext)
      result = true;

   return result;
}

bool AeroSimulatorEngine::CWin32Renderer::setRenderContext()
{
   // Save the current RC
   mOldRenderContext = wglGetCurrentContext();

   // Set the new RC
   return wglMakeCurrent(mDC, mRenderContext) ? true : false;
}

void AeroSimulatorEngine::CWin32Renderer::resetRenderContext()
{
   // Restore the context
   if (!wglMakeCurrent(mDC, mOldRenderContext))
      wglMakeCurrent(NULL, NULL);
}

bool AeroSimulatorEngine::CWin32Renderer::loadOpenGLExtensions()
{
   return false;
}
