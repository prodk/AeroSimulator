#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CShader.h"
#include "CGeometry.h"
#include "CLog.h"

#include <cassert>
using namespace AeroSimulatorEngine;

///@todo: move non-Win32 code to the base class

CWin32Renderer::CWin32Renderer(ePriority prio)
   : CRenderer(prio)
   , mDC(0)
   , mIsInitialized(false)
   , mRenderContext(0)
   , mOldRenderContext(0)
   , mIsFullScreen(false)
   , mVboId(0)
   , mIboId(0)
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

      glBindBuffer(GL_ARRAY_BUFFER, mVboId);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboId);

      glClearColor(0.95f, 0.95f, 0.95f, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      for (auto iter = std::begin(mRenderables); iter != std::end(mRenderables); ++iter)
      {
         CRenderable* pRenderable = *iter;
         if (pRenderable)
         {
            draw(pRenderable);
         }
      }

      swapBuffers();

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
         CLog::getInstance().log("* render context was destroyed");
      else
         CLog::getInstance().log("* ERROR: render context wasn't destroyed");
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

      ///@todo: think about the last argument
      glDrawElements(GL_TRIANGLE_STRIP, pGeometry->getNumOfIndices(), GL_UNSIGNED_INT, 0);
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
      CLog::getInstance().log("* pixel format was found: ", uiPixelFormat);
   }
   else
   {
      CLog::getInstance().log("* Error: pixel format wasn't found");
      return false;
   }

   // Set pixel format
   if (::SetPixelFormat(mDC, uiPixelFormat, &pfd))
   {
      CLog::getInstance().log("* pixel format was set");
   }
   else
   {
      CLog::getInstance().log("* Error: pixel format wasn't set");
      return false;
   }

   // Init GLEW
   HGLRC tempContext = wglCreateContext(mDC);

   wglMakeCurrent(mDC, tempContext);

   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      CLog::getInstance().log("GLEW is not initialized!");
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
   // Get the GPU information and the OpenGL extensions
   CLog::getInstance().log("* Video-system information:");
   CLog::getInstance().log("  Videocard: ", (const char*)glGetString(GL_RENDERER));
   CLog::getInstance().log("  Vendor: ", (const char*)glGetString(GL_VENDOR));
   CLog::getInstance().log("  OpenGL Version: ", (const char*)glGetString(GL_VERSION));
   CLog::getInstance().log("\n");

   //std::string strExtension = (const char*)glGetString(GL_EXTENSIONS);
   //ReplaceChar(strExtension, ' ', '\n');
   //Log::instance() << "  OpenGL Extensions:\n" << strExtension.c_str() << endl;

   return result;
}

bool CWin32Renderer::generateVBOs()
{
   setRenderContext();
   glEnable(GL_DEPTH_TEST);

   ///@todo: move to another method
   // Shader setup
   CShader* pShader = mRenderables[0]->getShader();
   pShader->link();
   pShader->setup(*mRenderables[0]);

   // VBO
   glGenBuffers(1, &mVboId);
   glBindBuffer(GL_ARRAY_BUFFER, mVboId);
   CLog::getInstance().logGL("* glBindBuffer() VBO: ");

   CGeometry* pGeometry = mRenderables[0]->getGeometry();
   assert(pGeometry);

   GLuint* data = static_cast<GLuint*>(pGeometry->getVertexBuffer());
   glBufferData(GL_ARRAY_BUFFER, pGeometry->getNumOfVertices()* sizeof(GLuint), data, GL_STATIC_DRAW);

   // Index buffer
   glGenBuffers(1, &mIboId);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboId);
   CLog::getInstance().log("* glBindBuffer() index buffer: ");

   GLuint* indices = (GLuint*)pGeometry->getIndexBuffer();
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, pGeometry->getNumOfIndices()* sizeof(GLuint), indices, GL_STATIC_DRAW);

   ///@todo: add resetting glBindBUffer here!
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   resetRenderContext();

   return true;
}
