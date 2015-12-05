#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "../src/shaders/CShader.h"
#include "CGeometry.h"
#include "CLog.h"
#include "CCommonMath.h" ///@todo: remove this, we can use glm::radians
#include "CCamera.h"
#include "CCompositeGameObject.h"
#include "CTimer.h"
#include "C3DModel.h"
#include "CSphere.h"
#include "CAnimationBillBoard.h"
#include "CLand.h"
#include "CBoundingBox.h"

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
   , mCameraAngleX(30.f)
   , mCameraAngleY(80.f)
   , mCamera(new CCamera())
   , mAirplaneRoot(nullptr)
   , mSphereRoot(nullptr)
   , mIsDebugMode(false)     // press 1
   , mIsSetCameraMode(false) // press 3
   , mCameraAttached(false)
   , mWndHandle(0)
   , mKeyPressed(false)
   , mCameraKeyPressed(false)
   , mThirdKeyPressed(false)
   , mKeyCode(0)
   , mCameraKeyCode(0)
   , mThirdKeyCode(0)
   , mFrameDt(0.0)
   , mAirplaneMatrix()
   , mStar(nullptr)
   , mLand(nullptr)
{
   assert(mCamera);

   mCamera->setProjectionMatrix(glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 500.0f));

   // View matrix.
   mCamera->setTranslate(glm::vec3(0.0f, 0.0f, -13.0f));
   mCamera->buildModelMatrix(glm::mat4x4(1.0f));
}

CWin32Renderer::~CWin32Renderer()
{
   destroy();
}

bool CWin32Renderer::start()
{
   return isInitialized();
}

void CWin32Renderer::update(CTask* pTask)
{
   if (mIsInitialized)
   {
      updateFPS(pTask);
      updateInput();    // Handle keyboard events

      setRenderContext();

      updateCamera();
      springButtons();

      updateRenderables();

      glClearColor(0.95f, 0.95f, 0.95f, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      for (auto * pRenderable : mRenderables)
      {
         if (pRenderable && pRenderable->canBeRendered() && pRenderable->isVisible())
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
}

void CWin32Renderer::draw(CRenderable* pRenderable)
{
   const GLuint vboId = pRenderable->getVboId();
   const GLuint iboId = pRenderable->getIboId();
   if (pRenderable && pRenderable->canBeRendered() && vboId && iboId)
   {
      glBindBuffer(GL_ARRAY_BUFFER, vboId);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

      pRenderable->setEnvironment(); /// Renderable-specific changes of the OpenGL environment: switch depth on/off etc.

      CGeometry* pGeometry = pRenderable->getGeometry();
      CShader* pShader = pRenderable->getShader();
      assert(pShader && pGeometry);

      // Calculate and set the final MVP matrix used in the shader
      glm::mat4 modelMatrix = pRenderable->getModelMatrix();
      glm::mat4 MVP = mCamera->getProjectionMatrix() * mCamera->getViewMatrix() * modelMatrix;
      pRenderable->setMvpMatrix(MVP);

      // Set shader attributes/uniforms
      pShader->setup(*pRenderable);

      if (mIsDebugMode && pRenderable->getDrawWithLines())
      {
         glLineWidth(pRenderable->getLineWidth());
         glDrawElements(GL_LINES, pGeometry->getNumOfIndices(), GL_UNSIGNED_INT, 0);
      }
      else if (!pRenderable->getDrawWithLines())
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
   glm::mat4 mAirplaneMatrix = glm::mat4(1.0f);
   if (mAirplane)
   {
      mAirplaneMatrix = glm::translate(mAirplaneMatrix, glm::vec3(0.f, mAirplane->getPosition().y, 0.f));
   }

   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   const float angleZradians = CCommonMath::degToRad(mAngleZ);
   mAirplaneMatrix = glm::rotate(mAirplaneMatrix, angleZradians, zAxis);

   // Rotate around x-axis
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   const float angleXradians = CCommonMath::degToRad(mAngleX);
   mAirplaneMatrix = glm::rotate(mAirplaneMatrix, angleXradians, xAxis);

   // Update the root and all its children.
   if (mAirplaneRoot)
   {
      mAirplaneRoot->updateTRMatrix(mAirplaneMatrix, mFrameDt); // Animate the parts of the tree-like object
      mAirplaneRoot->updateModelMatrix();
   }
}

void CWin32Renderer::updateCamera()
{
   mCamera->setRotate(glm::vec3(mCameraAngleX, mCameraAngleY, 0.f));
   mCamera->updateModelMatrix();
}

void CWin32Renderer::springButtons()
{
   // If the button was depressed, return the plane to the previous position
   const float rotationSpeed = 80.f*mFrameDt;
   if (!mKeyPressed
      || (mKeyPressed && mKeyCode != VK_LEFT && mKeyCode != VK_RIGHT))
   {
      if (mAngleZ > 0.0f)
      {
         mAngleZ -= rotationSpeed;
         mAngleZ = std::max<float>(0.0f, mAngleZ);
      }

      if (mAngleZ < -0.0f)
      {
         mAngleZ += rotationSpeed;
         mAngleZ = std::min<float>(0.0f, mAngleZ);
      }
   }

   ///@todo: check that no ground collision event happened
   if (!mKeyPressed
      || (mKeyPressed && mKeyCode != VK_UP))
   {
      if (mAirplane && mAirplane->getPosition().y > -11.f)
         mAirplane->decreasePropellerSpeed();
   }

   if (!mKeyPressed
      || (mKeyPressed && mKeyCode != VK_DOWN && mKeyCode != VK_UP))
   {
      // Put the plane tail down when not moving downwards
      if (mAngleX < 0.f)
      {
         mAngleX += rotationSpeed;
      }
   }

   if (!mThirdKeyPressed)
   {
      if (mAngleX > 0.f)
      {
         mAngleX -= rotationSpeed;
      }
   }
}

void CWin32Renderer::updateRenderables()
{
   updateAirplane();

   ///@todo: place to a method updateSphere
   if (mSphereRoot)
   {
      mSphereRoot->updateTRMatrix(glm::mat4x4(1.0f), mFrameDt);
      mSphereRoot->updateModelMatrix();
   }

   if (mStar)
   {
      mStar->update(mFrameDt);
   }

   CAnimationBillBoard* star = mStar.get();
   if (star)
   {
      star->updateTRMatrix(glm::mat4x4(1.0f), mFrameDt);
      star->updateModelMatrix();
   }

   handleCollisions();
}

void CWin32Renderer::updateFPS(CTask * pTask)
{
   if (pTask)
   {
      CTimer* pTimer = reinterpret_cast<CTimer*>(pTask);

      const int fps = pTimer->getFPS();
      mFrameDt = pTimer->getDtFrame();
      const double simDt = pTimer->getDtSim();

      wchar_t buf[256];
      swprintf(buf, L"FPS %d, frameDt %lf, simDt %lf", fps, mFrameDt, simDt);
      SetWindowText(mWndHandle, buf);
   }
}

void CWin32Renderer::updateInput()
{
   const float rotationSpeed = 50.f*mFrameDt;
   const float translateSpeed = 4.f*mFrameDt;
   if (mKeyPressed)
   {
      switch (mKeyCode)
      {
         /// Airplane rotations
      case (VK_LEFT) :
         mAngleZ += rotationSpeed;
         mAngleZ = std::min<float>(mAngleZ, 90.f);
         break;

      case (VK_RIGHT) :
         mAngleZ -= rotationSpeed;
         mAngleZ = std::max<float>(mAngleZ, -90.f);
         break;
         /// Airplane movement
      case (VK_UP) :
         if (mAirplane)
         {
            mAirplane->increasePropellerSpeed();
            if (mThirdKeyPressed && (mThirdKeyCode == VK_SPACE))
            {
               mAngleX += rotationSpeed;
               mAngleX = std::min<float>(mAngleX, 20.f);

               // Move upwards
               glm::vec3 position = mAirplane->getPosition();
               ///@todo: move to handle collisions
               position.y = std::min<float>(25.f, position.y + mAirplane->getSpeedOfFlight().y*mFrameDt);
               mAirplane->setPosition(position);
            }
         }
         break;

      case (VK_DOWN) :
      {
         if (mAirplane)
         {
            mAngleX -= rotationSpeed;
            mAngleX = std::max<float>(mAngleX, -20.f);

            // Move the plane downwards
            glm::vec3 position = mAirplane->getPosition();
            position.y = position.y - mAirplane->getSpeedOfFlight().y*mFrameDt;
            mAirplane->setPosition(position);
         }
      }
         break;

         /// System changes in reaction to the keyboard
      case (0x31) : // 1, debug mode on
         mIsDebugMode = true;
         break;

      case (0x32) : // 2, debug mode off
         mIsDebugMode = false;
         break;

      case (VK_OEM_PLUS) : // +, zoom in
         {
            // How it works:
            // i) take camera direction in the world space
            // ii) transform this direction to the camera space using the view matrix without translation
            // iii) move the scene along the direction in camera space;
            glm::vec3 direction = glm::cross(mCamera->getRightVector(), mCamera->getUpVector());
            glm::mat3x3 noTranslate = mCamera->getRotationMatrix();

            ///!Important: camera movement must be in camera space!
            direction = noTranslate*direction;
            direction = glm::normalize(direction);
            glm::vec3 translate = mCamera->getTranslate();
            translate += translateSpeed*direction;
           
            mCamera->setTranslate(translate);
         }
         break;

      case (VK_OEM_MINUS) : // -, zoom out
         {
            glm::vec3 direction = glm::cross(mCamera->getRightVector(), mCamera->getUpVector());
            glm::mat3x3 noTranslate = mCamera->getRotationMatrix();

            ///!Important: camera movement must be in camera space!
            direction = noTranslate*direction;
            direction = glm::normalize(direction);
            glm::vec3 translate = mCamera->getTranslate();
            translate -= translateSpeed*direction;

            mCamera->setTranslate(translate);
         }
         break;
      }
   }

   // Camera rotations
   if (mCameraKeyPressed)
   {
      switch (mCameraKeyCode)
      {
         // All the shifts are in camera space
      case (0x57) : // w, up
         if (mIsSetCameraMode)
         {
            glm::vec3 translate(0.f, -translateSpeed, 0.f);
            mCamera->translateLookAt(translate);
         }
         else
         {
            mCameraAngleX += rotationSpeed;
            if (mCameraAngleX >= 360.f) mCameraAngleX = 0.f;
         }
         break;

      case (0x53) : // s, down
         if (mIsSetCameraMode)
         {
            glm::vec3 translate(0.f, translateSpeed, 0.f);
            mCamera->translateLookAt(translate);
         }
         else
         {
            mCameraAngleX -= rotationSpeed;
            if (mCameraAngleX <= -360.f) mCameraAngleX = 0.f;
         }
         break;

      case (0x41) : // a, left
         /// Move camera horizontally if the setup mode is on
         if (mIsSetCameraMode)
         {
            glm::vec3 translate (translateSpeed, 0.f, 0.f);
            mCamera->translateLookAt(translate);
         }
         else // Otherwise, rotate the camera
         {
            mCameraAngleY += rotationSpeed;
            if (mCameraAngleY >= 360.f) mCameraAngleY = 0.f;
         }
         break;

      case (0x44) : // d, right
         /// Move camera horizontally if the setup mode is on
         if (mIsSetCameraMode)
         {
            glm::vec3 translate(-translateSpeed, 0.f, 0.f);
            mCamera->translateLookAt(translate);
         }
         else // Otherwise, rotate the camera
         {
            mCameraAngleY -= rotationSpeed;
            if (mCameraAngleY <= -360.f) mCameraAngleY = 0.f;
         }
         break;
      }
   }
}

void CWin32Renderer::handleCollisions()
{
   ///@todo: don't do this update all the time
   ///@todo: do these checks and actions when the distance is small enough
   ///@todo: use oct or quad trees to search for neighboring game objects

   /// Airplane vs land collisions
   ///@todo: put to a separate method
   if (mAirplane && mLand)
   {
      const CBoundingBox* boxAirPlane = mAirplane->getBoundingBox();
      const CBoundingBox* boxLand = mLand->getBoundingBox();
      if (boxAirPlane && boxLand)
      {
         if (boxAirPlane->collidesWith(*boxLand))
         {
            mAirplane->resetHealthBars();
            mAirplane->setPropellerSpeed(0.0f);
            // Restore the previous position of the plane
            glm::vec3 newPos = mAirplane->getPosition();
            newPos.y = newPos.y + mAirplane->getSpeedOfFlight().y*mFrameDt;
            mAirplane->setPosition(newPos);
         }
      }
   }

   /// Airplane vs stars collisions
   ///@todo: put to a separate method
   if (mAirplane && mStar)
   {
      const CBoundingBox* boxAirPlane = mAirplane->getBoundingBox();
      const CBoundingBox* boxStar = mStar->getBoundingBox();
      if (boxAirPlane && boxStar)
      {
         if (boxAirPlane->collidesWith(*boxStar))
         {
            mAirplane->resetHealthBars(0.8);
            mStar->setVisible(false);
         }
         else
         {
            mStar->setVisible(true);
         }
      }
   }
}

bool CWin32Renderer::windowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
   switch (uMessage)
   {
   case WM_CREATE:
   {
      mWndHandle = hWnd;
      SetWindowText(hWnd, L"Hi there!");
   }
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
      // w, s, a, d - camera key strokes, move camera independently of whether other keys are pressed
      if ((wParam == 0x57) || (wParam == 0x53) || (wParam == 0x41) || (wParam == 0x44))
      {
         mCameraKeyPressed = true;
         mCameraKeyCode = wParam;
      }
      else if (!mKeyPressed)
      {
         mKeyPressed = true;
         mKeyCode = wParam;
      }
      else if(mKeyCode == VK_UP)// Only if UP was pressed
      {
         mThirdKeyPressed = true;
         mThirdKeyCode = wParam;
      }

      switch (wParam)
      {
      case (0x33) : // 3, camera setup mode on
         mIsSetCameraMode = true;
         CLog::getInstance().log("* Button 3: Setup enabled! Move camera horiz a/d, vert w/s; attach/detach Enter ");
         break;

      case (0x34) : // 4, camera setup mode on
         mIsSetCameraMode = false;
         CLog::getInstance().log("* Button 4: Setup disabled! Rotate camera horiz a/d, vert w/s ");
         break;

      case (0x35) : // 5, reset all healthbars to some value
         if (mAirplane)
         {
            mAirplane->resetHealthBars(0.3f);
            CLog::getInstance().log("* Button 5: Reset all healthbars.");
         }
         break;

      case (0x36) : // 6, set look at at 000
         if (mCamera)
         {
            mCamera->resetLookAt();
            if (mIsSetCameraMode)
               CLog::getInstance().log("* Button 6: Set look at point to the planes cabine");
            else
               CLog::getInstance().log("* Button 6: Set look at point to the center of the scene");
         }
         break;

      case (VK_RETURN) : // Enter, attach the camera to the airplane
         if (mIsSetCameraMode)
         {
            if (!mCameraAttached)
            {
               mAirplaneRoot->add(mCamera.get());
               mAirplaneRoot->buildModelMatrix(glm::mat4x4(1.0f));
               mCameraAttached = true;
               CLog::getInstance().log("* Button Enter: Camera attached to the plane");
            }
            else
            {
               mAirplaneRoot->remove(mCamera.get());
               mCameraAttached = false;
               CLog::getInstance().log("* Button Enter:Camera detached from the plane");
            }
         }
         break;
      } // end switch (wParam)
   }
   return false;

   // Keyboard is depressed
   case WM_KEYUP:
   {
      if ((wParam == 0x57) || (wParam == 0x53) || (wParam == 0x41) || (wParam == 0x44))
         mCameraKeyPressed = false;
      else //if (mKeyPressed && (wParam == mThirdKeyCode))
      {
         mThirdKeyPressed = false;

         //if (wParam == mKeyCode)
            mKeyPressed = false;
      }
   }
   return false;
   } // end switch

   // Process other messages
   return true;
}
