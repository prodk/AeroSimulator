#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "../src/shaders/CShader.h"
#include "CGeometry.h"
#include "CLog.h"
#include "CCommonMath.h" ///@todo: remove this
#include "CCamera.h"
#include "CCompositeGameObject.h"

#include "glm/gtc/matrix_transform.hpp"

#include <cassert>
#include <algorithm>
using namespace AeroSimulatorEngine;

// Comparator for sorting renderables
//struct SCompareRenderables
//{
//   bool operator()(CRenderable* lhs, CRenderable* rhs)
//   {
//      if (lhs && rhs)
//      {
//         // get the distance to camera and return lhsDist < rhsDist;
//            return a < b;
//      }
//   }
//};

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
   , mCameraAngleX(30.f)
   , mCameraAngleY(80.f)
   , mCamera(new CCamera())
   , mAirplaneRoot(nullptr)
   , mSphereRoot(nullptr)
   , mIsDebugMode(false)
   , mCameraScale(1.0f)
{
   assert(mCamera);

   mCamera->setProjectionMatrix(glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 500.0f));

   // View matrix.
   mCamera->translate(glm::vec3(0.0f, 0.0f, -13.0f));
}

CWin32Renderer::~CWin32Renderer()
{
   destroy();
}

bool CWin32Renderer::start()
{
   return isInitialized();
}

void CWin32Renderer::update()
{
   if (mIsInitialized)
   {
      setRenderContext();

      updateCamera();
      springButtons();

      updateRenderables();

      glClearColor(0.95f, 0.95f, 0.95f, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      for (auto * pRenderable : mRenderables)
      {
         if (pRenderable && pRenderable->canBeRendered())
         {
            ///todo: think how to set the width/height of the billboard
            pRenderable->setRightVector(mCamera->getRightVector());
            pRenderable->setUpVector(mCamera->getUpVector());
            pRenderable->setEyePos(mCamera->getPositionWorldSpace());
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
   if (createRenderContext() && setRenderContext() && loadOpenGLExtensions())
   {
      mIsInitialized = true;

      // Enable OpenGL stuff needed by the app
      glEnable(GL_DEPTH_TEST);
      CLog::getInstance().logGL("Depth test enabled: ");

      glEnable(GL_TEXTURE_2D);
      CLog::getInstance().logGL("Textures enabled: ");

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // Go back to the window rendering context
      resetRenderContext();
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

   //mRoot = nullptr;
}

void CWin32Renderer::draw(CRenderable* pRenderable)
{
   const GLuint vboId = pRenderable->getVboId();
   const GLuint iboId = pRenderable->getIboId();
   if (pRenderable && pRenderable->canBeRendered() && vboId && iboId)
   {
      glBindBuffer(GL_ARRAY_BUFFER, vboId);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

      pRenderable->setEnvironment(); /// Renderable specific changes of the OpenGL environment: switch depth on/off etc.

      CGeometry* pGeometry = pRenderable->getGeometry();
      CShader* pShader = pRenderable->getShader();
      assert(pShader && pGeometry);

      // Calculate and set the final MVP matrix used in the shader
      glm::mat4 modelMatrix = pRenderable->getModelMatrix();
      glm::mat4 MVP = mCamera->getProjectionMatrix() * mCamera->getViewMatrix() * modelMatrix;
      pRenderable->setMvpMatrix(MVP);

      // Set shader attributes/uniforms
      pShader->setup(*pRenderable);

      //GLenum mode = GL_TRIANGLE_STRIP;
      
      if (mIsDebugMode && pRenderable->getDrawWithLines())
      {
         glLineWidth(pRenderable->getLineWidth());
         glDrawElements(GL_LINES, pGeometry->getNumOfIndices(), GL_UNSIGNED_INT, 0);
      }
      else
         glDrawElements(GL_TRIANGLE_STRIP, pGeometry->getNumOfIndices(), GL_UNSIGNED_INT, 0);

      // Return to the initial OpenGL state.
      pRenderable->resetEnvironment();

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

void CWin32Renderer::updateAirplane()
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

   //matrix = modelObjectMatrix;

   // Update the root and all its children.
   if (mAirplaneRoot)
   {
      mAirplaneRoot->updateTRMatrix(glm::mat4(1.0f)); // Animate the parts of the tree-like object
      mAirplaneRoot->updateModelMatrix(modelObjectMatrix);
   }
}

void CWin32Renderer::updateCamera()
{
   mCamera->rotate(glm::vec3(mCameraAngleX, mCameraAngleY, 0.f));
   mCamera->update();
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

void CWin32Renderer::updateRenderables()
{
   updateAirplane();

   ///@todo: place to a method updateSphere
   mSphereRoot->updateTRMatrix(glm::mat4x4(1.0f));
   mSphereRoot->updateModelMatrix(glm::mat4x4(1.0f));
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
         if (mCameraAngleX >= 360.f) mCameraAngleX = 0.f;
      }

      if (wParam == 0x53) // s, down
      {
         mCameraAngleX -= 1.f;
         if (mCameraAngleX <= -360.f) mCameraAngleX = 0.f;
      }

      if (wParam == 0x41) // a, left
      {
         mCameraAngleY += 1.f;
         if (mCameraAngleY >= 360.f) mCameraAngleY = 0.f;
      }

      if (wParam == 0x44) // d, right
      {
         mCameraAngleY -= 1.f;
         if (mCameraAngleY <= -360.f) mCameraAngleY = 0.f;
      }

      if (wParam == 0x31) // 1, debug mode on
      {
         mIsDebugMode = true;
      }

      if (wParam == 0x32) // 2, debug mode off
      {
         mIsDebugMode = false;
      }

      if (wParam == VK_OEM_PLUS) // +, zoom in
      {
         mCameraScale += 0.02f;
         mCamera->scale(glm::vec3(mCameraScale, mCameraScale, mCameraScale));
      }

      if (wParam == VK_OEM_MINUS) // -, zoom out
      {
         mCameraScale -= 0.02f;
         mCameraScale = std::max(0.1f, mCameraScale);
         mCamera->scale(glm::vec3(mCameraScale, mCameraScale, mCameraScale));
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
