#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CShader.h"
#include "CGeometry.h"
#include "CLog.h"

#include "glm/gtc/matrix_transform.hpp"

#include <cassert>
using namespace AeroSimulatorEngine;

///@todo: move non-Win32 code to the base class

///@todo: remove these defines (put them to one place)
namespace AeroSimulatorEngine
{
   ///@todo: make these static constants
   #define M_PI           3.14159265358979323846f  /* pi */
   #define DEG_TO_RAD M_PI / 180.f
}

CWin32Renderer::CWin32Renderer(ePriority prio)
   : CRenderer(prio)
   , mDC(0)
   , mIsInitialized(false)
   , mRenderContext(0)
   , mOldRenderContext(0)
   , mIsFullScreen(false)
   , mViewMatrix()
   , mProjectionMatrix(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f)) // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
   , mAngleZ(0.0f)
   , mAngleX(0.0f)
{
}

CWin32Renderer::~CWin32Renderer()
{
   destroy();
}

///@todo: probably move to the base class
bool CWin32Renderer::start()
{
   return isInitialized();
}

///@todo: probably move to the base class
void CWin32Renderer::update()
{
   if (mIsInitialized)
   {
      setRenderContext();

      setupViewMatrix();

      glm::mat4 modelObjectMatrix;
      calculateAirplaneMatrix(modelObjectMatrix);

      glClearColor(0.95f, 0.95f, 0.95f, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      for (auto iter = std::begin(mRenderables); iter != std::end(mRenderables); ++iter)
      {
         CRenderable* pRenderable = *iter;
         if (pRenderable)
         {
            pRenderable->setParentModelMatrix(modelObjectMatrix);
            draw(pRenderable);
         }
      }

      swapBuffers();

      resetRenderContext();
   }
}

void CWin32Renderer::stop()
{
}

void CWin32Renderer::init()
{
   if (createRenderContext())
   {
      if (setRenderContext() && loadOpenGLExtensions())
      {
         mIsInitialized = true;

         // Enable OpenGL stuff needed by the app
         glEnable(GL_DEPTH_TEST);

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

///@todo: probably move to the base class
void CWin32Renderer::draw(CRenderable* pRenderable)
{
   if (pRenderable)
   {
      glBindBuffer(GL_ARRAY_BUFFER, pRenderable->getVboId());
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pRenderable->getIboId());

      CGeometry* pGeometry = pRenderable->getGeometry();
      CShader* pShader = pRenderable->getShader();

      assert(pShader && pGeometry);

      // Get Renderable's static model matrix
      glm::mat4 modelMatrix = pRenderable->getModelMatrix();

      // Get Renderable's dynamic library of the root object
      glm::mat4 modelObjectMatrix = pRenderable->getParentModelMatrix();

      // Calculate and set the final MVP matrix used in the shader
      glm::mat4 MVP = mProjectionMatrix * mViewMatrix * modelObjectMatrix * modelMatrix;
      pRenderable->setMvpMatrix(MVP);

      pShader->setup(*pRenderable);

      glDrawElements(GL_TRIANGLE_STRIP, pGeometry->getNumOfIndices(), GL_UNSIGNED_INT, 0);

      // Unbind the buffers
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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


///@todo: probably move to the base class
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

void CWin32Renderer::setupViewMatrix()
{
   // Init the View matrix
   mViewMatrix = glm::mat4(1.0f);
   glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -10.0f);
   mViewMatrix = glm::translate(mViewMatrix, cameraPos);

   // Rotate the View matrix
   static float angle = 0.f * DEG_TO_RAD;
   static float angleX = 30.0f * DEG_TO_RAD;

   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   //View = glm::rotate(View, angle, yAxis);
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   mViewMatrix = glm::rotate(mViewMatrix, angleX, xAxis);

   const float delta = 0.005f;
   angle += delta;
}

void CWin32Renderer::calculateAirplaneMatrix(glm::mat4& matrix) const
{
   glm::mat4 modelObjectMatrix = glm::mat4(1.0f);
   
   // Rotate around z-axis
   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   const float angleZradians = mAngleZ * DEG_TO_RAD;

   modelObjectMatrix = glm::rotate(modelObjectMatrix, angleZradians, zAxis);

   // Rotate around x-axis
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   const float angleXradians = mAngleX * DEG_TO_RAD;
   modelObjectMatrix = glm::rotate(modelObjectMatrix, angleXradians, xAxis);

   matrix = modelObjectMatrix;
}

bool CWin32Renderer::windowProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
   switch (uMessage)
   {
      // Stubs for system commands
   case WM_SYSCOMMAND:
   {
      switch (wParam)
      {
         // Screensaver is trying to start
      case SC_SCREENSAVE:
         // The display is trying to switch off
      case SC_MONITORPOWER:
         return false;
      }
      break;
   }
   return false;

   // System keystrokes stubs
   case WM_SYSKEYDOWN:
   case WM_SYSKEYUP:
   {}
   return false;

   // Finishing
   case WM_CLOSE:
   case WM_QUIT:
   {
      // Exit flag
      //mIsClosing = true;
   }
   return false;

   // Keyboard is pressed
   case WM_KEYDOWN:
   {
      if (wParam == VK_LEFT)
      {
         if (mAngleZ < 45)
            mAngleZ += 1.0f;
         //CLog::getInstance().log("Left pressed");
      }
      if (wParam == VK_RIGHT)
      {
         if (mAngleZ > -45)
            mAngleZ -= 1.0f;

         //CLog::getInstance().log("Right pressed");
      }
      if (wParam == VK_UP)
      {
         if (mAngleX > -45)
            mAngleX -= 1.0f;
         //CLog::getInstance().log("Up pressed");
      }
      if (wParam == VK_DOWN)
      {
         if (mAngleX < 45)
            mAngleX += 1.0f;
         //CLog::getInstance().log("Down pressed");
      }
   }
   return false;

   // Keyboard is depressed
   case WM_KEYUP:
   {
      if (wParam == VK_LEFT)
      {
         int pressed = 1;
         CLog::getInstance().log("Left DEpressed");
      }
      if (wParam == VK_RIGHT)
      {
         int pressed = 0;
         CLog::getInstance().log("Right DEpressed");
      }
      if (wParam == VK_UP)
      {
         int pressed = 1;
         CLog::getInstance().log("Up DEpressed");
      }
      if (wParam == VK_DOWN)
      {
         int pressed = 0;
         CLog::getInstance().log("Down DEpressed");
      }
   }
   return false;
   } // end switch

     // Process other messages
   return true;
}
