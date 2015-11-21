#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CShader.h"
#include "CGeometry.h"
#include "CLog.h"
#include "CCommonMath.h" ///@todo: remove this
#include "CCamera.h"
#include "CGameObject.h"

#include "glm/gtc/matrix_transform.hpp"

#include <cassert>
#include <algorithm>
using namespace AeroSimulatorEngine;

///@todo: move non-Win32 code to the base class

CWin32Renderer::CWin32Renderer(ePriority prio)
   : CRenderer(prio)
   , mDC(0)
   , mIsInitialized(false)
   , mRenderContext(0)
   , mOldRenderContext(0)
   , mIsFullScreen(false)
   , mAngleZ(0.0f)
   , mAngleX(0.0f)
   , mHorizontalPressed(0)
   , mVerticalPressed(0)
   , mCameraAngleX(0.f) // up 'w', down 's'
   , mCameraAngleY(0.f)
   , mCamera(new CCamera())
   //, mCameraVerticalPressed(0)
   //, mCameraHorizontalPressed(0)
   , mRoot(nullptr)
   , mDynamicMatrix()
{
   assert(mCamera);

   mCamera->setProjectionMatrix(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f));

   // View matrix.
   mCamera->translate(glm::vec3(0.0f, 0.0f, -15.0f));
   mCamera->rotate(glm::vec3(0.0f, 0.0f, 0.0f));
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

void CWin32Renderer::update()
{
   if (mIsInitialized)
   {
      setRenderContext();

      ///@todo: rotate camera here if needed
      rotateCamera();
      springButtons();

      glm::mat4 modelObjectMatrix;
      calculateAirplaneMatrix(modelObjectMatrix);

      glClearColor(0.95f, 0.95f, 0.95f, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      for (auto iter = std::begin(mRenderables); iter != std::end(mRenderables); ++iter)
      {
         CRenderable* pRenderable = *iter;
         if (pRenderable && pRenderable->canBeRendered())
         {
            ///@todo: remove this
            //pRenderable->setParentModelMatrix(modelObjectMatrix);
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
         CLog::getInstance().logGL("Depth test enabled: ");

         glEnable(GL_TEXTURE_2D);
         CLog::getInstance().logGL("Textures enabled: ");

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

   mRoot = nullptr;
}

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

      ///@todo: do not multiply by the parent matrix each frame!!! Do this only if it has changed.
      // Get Renderable's dynamic library of the root object
      //glm::mat4 modelObjectMatrix = pRenderable->getParentModelMatrix();

      // Calculate and set the final MVP matrix used in the shader
      //glm::mat4 MVP = mCamera->getProjectionMatrix() * mCamera->getViewMatrix() * modelObjectMatrix * modelMatrix;

      // Calculate and set the final MVP matrix used in the shader
      glm::mat4 MVP = mCamera->getProjectionMatrix() * mCamera->getViewMatrix() * modelMatrix;
      pRenderable->setMvpMatrix(MVP);

      pShader->setup(*pRenderable);

      glDrawElements(GL_TRIANGLE_STRIP, pGeometry->getNumOfIndices(), GL_UNSIGNED_INT, 0);

      // Unbind the buffers
      ///@todo: probably place to a separate method
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      // Unbind the texture
      glBindTexture(GL_TEXTURE_2D, 0);
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

   CLog::getInstance().log("* Loading OpenGL extensions");
   std::string strExtension = (const char*)glGetString(GL_EXTENSIONS);
   std::replace(strExtension.begin(), strExtension.end(), ' ', ';');

   CLog::getInstance().log("  OpenGL Extensions:");
   CLog::getInstance().log(strExtension.c_str());

   // Get the GPU information and the OpenGL extensions
   CLog::getInstance().log("* Video-system information:");
   CLog::getInstance().log("  Videocard: ", (const char*)glGetString(GL_RENDERER));
   CLog::getInstance().log("  Vendor: ", (const char*)glGetString(GL_VENDOR));
   CLog::getInstance().log("  OpenGL Version: ", (const char*)glGetString(GL_VERSION));
   CLog::getInstance().log("\n");

   return result;
}

void CWin32Renderer::calculateAirplaneMatrix(glm::mat4& matrix)
{
   glm::mat4 modelObjectMatrix = glm::mat4(1.0f);

   // Rotate around z-axis
   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   const float angleZradians = CCommonMath::degToRad(mAngleZ);

   modelObjectMatrix = glm::rotate(modelObjectMatrix, angleZradians, zAxis);

   // Rotate around x-axis
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   const float angleXradians = CCommonMath::degToRad(mAngleX);
   modelObjectMatrix = glm::rotate(modelObjectMatrix, angleXradians, xAxis);

   matrix = modelObjectMatrix;

   ///Rotate the dynamic part (propeller)
   static float angle;
   const float delta = 0.1f;
   mDynamicMatrix = glm::mat4(1.0f);
   mDynamicMatrix = glm::rotate(mDynamicMatrix, angle, zAxis);
   angle += delta;

   // Update the root and all its children.
   //mRoot->updateMatrix(modelObjectMatrix, mDynamicMatrix);
   mRoot->updateTRMatrix(glm::mat4(1.0f)); // Animate the parts of the tree-like object
   mRoot->updateModelMatrix(modelObjectMatrix);
}

void CWin32Renderer::rotateCamera()
{
   mCamera->resetView();

   mCamera->rotate(glm::vec3(mCameraAngleX, mCameraAngleY, 0.f));
   mCamera->translate(glm::vec3(0.f, 0.f, -10.f));
}

void CWin32Renderer::springButtons()
{
   // If the button was depressed, return the plane to the previous position
   if (mHorizontalPressed)
   {
      if (mAngleZ > 0.0f)
      {
         mAngleZ -= 0.8f;
         mAngleZ = std::max<float>(0.0f, mAngleZ);
      }

      if (mAngleZ < -0.0f)
      {
         mAngleZ += 0.8f;
         mAngleZ = std::min<float>(0.0f, mAngleZ);
      }
   }

   if (mVerticalPressed)
   {
      if (mAngleX > 0.f)
         mAngleX -= 0.4f;

      if (mAngleX < 0.f)
         mAngleX += 0.4f;
   }
}

bool CWin32Renderer::windowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
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

   ///@todo: probably unnecessary as it is present in CWin32Window
   // Finishing
   case WM_CLOSE:
   case WM_QUIT:
   {
   }
   return false;

   // Keyboard is pressed
   case WM_KEYDOWN:
   {
      if (wParam == VK_LEFT)
      {
         mAngleZ += 1.0f;
         mAngleZ = std::min<float>(mAngleZ, 45.f);
         mHorizontalPressed = 0;
      }
      if (wParam == VK_RIGHT)
      {
         mAngleZ -= 1.0f;
         mAngleZ = std::max<float>(mAngleZ, -45.f);
         mHorizontalPressed = 0;
      }
      if (wParam == VK_UP)
      {
         mAngleX -= 1.0f;
         mAngleX = std::max<float>(mAngleX, -45.f);
         mVerticalPressed = 0;
      }
      if (wParam == VK_DOWN)
      {
         mAngleX += 1.0f;
         mAngleX = std::min<float>(mAngleX, 45.f);
         mVerticalPressed = 0;
      }

      // Camera rotations
      if (wParam == 0x57) // w, up
      {
         mCameraAngleX += 1.f;
         mCameraAngleX = std::min<float>(mCameraAngleX, 360.f);
      }

      if (wParam == 0x53) // s, down
      {
         mCameraAngleX -= 1.f;
         mCameraAngleX = std::max<float>(mCameraAngleX, -360.f);
      }

      if (wParam == 0x41) // a, left
      {
         mCameraAngleY += 1.f;
         mCameraAngleY = std::min<float>(mCameraAngleY, 360.f);
      }

      if (wParam == 0x44) // d, right
      {
         mCameraAngleY -= 1.f;
         mCameraAngleY = std::max<float>(mCameraAngleY, -360.f);
      }
   }
   return false;

   // Keyboard is depressed
   case WM_KEYUP:
   {
      if (wParam == VK_LEFT)
      {
         ++mHorizontalPressed;
      }
      if (wParam == VK_RIGHT)
      {
         ++mHorizontalPressed;
      }
      if (wParam == VK_UP)
      {
         ++mVerticalPressed;
      }
      if (wParam == VK_DOWN)
      {
         ++mVerticalPressed;
      }
   }
   return false;
   } // end switch

   // Process other messages
   return true;
}
