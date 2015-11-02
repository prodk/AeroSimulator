#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CShader.h"
#include "CGeometry.h"
#include <cassert>
using namespace AeroSimulatorEngine;

CWin32Renderer::CWin32Renderer(ePriority prio)
   : CRenderer(prio)
   , mDC(0)
   , mIsInitialized(false)
   , mRenderContext(0)
   , mOldRenderContext(0)
   , mIsFullScreen(false)
{
}

CWin32Renderer::~CWin32Renderer()
{
   destroy();
}

bool CWin32Renderer::start()
{
   return isInitialized();
}

bool CWin32Renderer::update()
{
   if (mIsInitialized)
   {
      setRenderContext();

      glClearColor(0.95f, 0.0f, 0.0f, 1);
      glClear(GL_COLOR_BUFFER_BIT);
      for (auto iter = std::begin(mRenderables); iter != std::end(mRenderables); ++iter)
      {
         CRenderable* pRenderable = *iter;
         if (pRenderable)
         {
            draw(pRenderable);
         }
      }

      swapBuffers();

      resetRenderContext();
   }

   return true;
}

bool CWin32Renderer::stop()
{
   return false;
}

void CWin32Renderer::init()
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

void CWin32Renderer::destroy()
{
   wglMakeCurrent(NULL, NULL);

   if (mRenderContext)
   {
      if (wglDeleteContext(mRenderContext))
         std::cout << "* render context was destroyed" << std::endl;
      else
         std::cout << "* ERROR: render context wasn't destroyed" << std::endl;
      mRenderContext = NULL;
   }
}

void CWin32Renderer::draw(CRenderable* pRenderable)
{
   if (pRenderable)
   {
      CGeometry* pGeometry = pRenderable->getGeometry();
      CShader* pShader = pRenderable->getShader();

      assert(pShader && pGeometry);

      pShader->setup(*pRenderable);
     /* glDrawElements(
         GL_TRIANGLES,
         pGeometry->GetNumIndices(),
         GL_UNSIGNED_SHORT,
         pGeometry->GetIndexBuffer());*/
      glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, 0);
   }
}

void CWin32Renderer::swapBuffers()
{
   ::SwapBuffers(mDC);
}

void CWin32Renderer::init(const CWin32Window & window)
{
   mDC = window.getDC();
   mIsFullScreen = window.isFullScreen();
   init();
}

bool CWin32Renderer::createRenderContext()
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
   wglMakeCurrent(mDC, tempContext);

   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      /*Log::instance() << "GLEW is not initialized!" << endl;*/
      std::cout << "GLEW is not initialized!" << std::endl;
   }

   int attribs[] =
   {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
      WGL_CONTEXT_MINOR_VERSION_ARB, 1,
      WGL_CONTEXT_FLAGS_ARB, 0,
      0
   };

   if (wglewIsSupported("WGL_ARB_create_context") == 1)
   {
      mRenderContext = wglCreateContextAttribsARB(mDC, 0, attribs);
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(tempContext);
      //wglMakeCurrent(m_hDC, m_hRC); // Making the context current is done in setRC
   }
   else
   {  //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
      mRenderContext = tempContext;
   }

   if (mRenderContext)
      result = true;

   return result;
}

bool CWin32Renderer::setRenderContext()
{
   // Save the current RC
   mOldRenderContext = wglGetCurrentContext();

   // Set the new RC
   return wglMakeCurrent(mDC, mRenderContext) ? true : false;
}

void CWin32Renderer::resetRenderContext()
{
   // Restore the context
   if (!wglMakeCurrent(mDC, mOldRenderContext))
      wglMakeCurrent(NULL, NULL);
}

bool CWin32Renderer::loadOpenGLExtensions()
{
   bool result = true;
   // получаем строку с информацией о videocard
   std::cout << "* Video-system information:" << std::endl;
   std::cout << "  Videocard: " << (const char*)glGetString(GL_RENDERER) << std::endl;
   std::cout << "  Vendor: " << (const char*)glGetString(GL_VENDOR) << std::endl;
   std::cout << "  OpenGL Version: " << (const char*)glGetString(GL_VERSION) << std::endl << std::endl;
   //std::string strExtension = (const char*)glGetString(GL_EXTENSIONS);
   //ReplaceChar(strExtension, ' ', '\n');
   //Log::instance() << "  OpenGL Extensions:\n" << strExtension.c_str() << endl;

   //// Load shader manually.
   //// TODO: check pointers for 0
   //glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
   //glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
   //glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
   //glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
   //glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
   //glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
   //glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
   //glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
   //glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
   //glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

   //glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
   //glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
   //glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
   //glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");

   //glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
   //glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");

   //std::cout << "  OpenGL Extensions loaded manually. Stupid, but we have to do it this way.\n" << std::endl;

   return result;
}
