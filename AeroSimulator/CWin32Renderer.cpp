#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CShader.h"
#include "CGeometry.h"
#include "CLog.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
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

      ///@todo: place matrix manipulation here from CSimpleShader::rotateCamera()! then set the modified general matrix to the shader.

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

      ///@todo: place matrix manipulation to update! then set the modified general matrix to the shader.
      ///@todo: move to separate method

      ///@todo: move setting up View and Projection to update(); introduce these matrices as well as modelObject to Renderer.
      // Init the View matrix
      glm::mat4 View = glm::mat4(1.0f);
      glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -9.0f);
      View = glm::translate(View, cameraPos);

      // Rotate the View matrix
      static float angle = 0.f * DEG_TO_RAD;
      static float angleX = 30.0f * DEG_TO_RAD;

      glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
      //View = glm::rotate(View, angle, yAxis);
      glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
      View = glm::rotate(View, angleX, xAxis);

      const float delta = 0.005f;
      angle += delta;

      ///@todo: move projection to construction
      // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
      glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

      glm::mat4 Model = pRenderable->getModelMatrix();
      glm::mat4 modelObject = glm::mat4(1.0f);
      modelObject = glm::rotate(modelObject, angle, yAxis);

      glm::mat4 MVP = Projection * View * modelObject * Model;
      pRenderable->setMvpMatrix(MVP);

      ///@todo: end move to separate method

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

