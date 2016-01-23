#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CEventManager.h"
#include "../Test/CGame.h" ///@todo: probably remove this when event declaration moved to other place, renderer should not refer to game

#include "../src/shaders/CShader.h"
//#include "CGeometry.h"
#include "CLog.h"
#include "CCommonMath.h" ///@todo: remove this, we can use glm::radians
#include "CCamera.h"
//#include "CCompositeGameObject.h"
#include "CTimer.h"
//#include "C3DModel.h"
//#include "CSphere.h"
//#include "CAnimationBillBoard.h"
//#include "../Test/CLand.h"
//#include "CBoundingBox.h"
//#include "CSkyBox.h"
//#include "CParticleSystem.h"
#include "CQuad.h"
#include "../src/shaders/CFboShader.h"
#include "../src/shaders/CDepthBufferShader.h"
//#include "CTexture.h"
//#include "CMissile.h"

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
   //, mAngleZ(0.0f)
   //, mAngleX(0.0f)
   , mCameraAngleX(14.f)
   , mCameraAngleY(0.f)
   , mCamera(new CCamera())
   //, mAirplaneRoot(nullptr)
   //, mSphereRoot(nullptr)
   , mIsDebugMode(false)     // press '1' key
   , mIsSetCameraMode(false) // press '3' key
   , mCameraAttached(false)
   , mWndHandle(0)
   //, mKeyPressed(false)
   //, mCameraKeyPressed(false)
   //, mThirdKeyPressed(false)
   //, mKeyCode(0)
   //, mCameraKeyCode(0)
   //, mThirdKeyCode(0)
   , mFrameDt(0.0)
   //, mAirplaneMatrix()
   //, mStar()
   //, mLand(nullptr)
   //, mSky(nullptr)
   //, mTurbineFire(nullptr)
   //, mTurbineSmoke(nullptr)
   , mMainFboQuad(new CQuad())
   , mHelpFboQuad(new CQuad())
   , mFboShader(new CFboShader())
   , mDepthBufferShader(new CDepthBufferShader())
   , mMainFbo()
   , mHelpFbo()
   , mWndWidth(0)
   , mWndHeight(0)
   , mDepthBufferMode(false)
   //, mRightMissile(nullptr)
   //, mExplosion(nullptr)
{
   assert(mCamera);
   assert(mMainFboQuad);
   assert(mHelpFboQuad);
   assert(mFboShader);
   assert(mDepthBufferShader);

   mCamera->setProjectionMatrix(glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 500.0f));

   // View matrix.
   mCamera->setTranslate(glm::vec3(0.0f, 0.0f, -10.0f));
   mCamera->buildModelMatrix(glm::mat4x4(1.0f));
}

CWin32Renderer::~CWin32Renderer()
{
   destroy();
}

bool CWin32Renderer::start()
{
   setupEvents();

   return isInitialized();
}

void CWin32Renderer::update(CTask* pTask)
{
   if (mIsInitialized)
   {
      updateFPS(pTask);
      updateInput();    // Handle keyboard events
      springButtons();

      updateRenderables();
      updateCamera();

      setRenderContext();

      // Render the scene to the main frame buffer
      glBindFramebuffer(GL_FRAMEBUFFER, mMainFbo.mFramebuffer);
      glViewport(0, 0, mWndWidth, mWndHeight);
      ///@todo: probably set textures only for some conditions to avoid doing this every frame
      mMainFboQuad->getTexture()->setId(mMainFbo.mTexColorBuffer);
      mMainFboQuad->setShadersAndBuffers(mFboShader);
      drawScene();

      ///@todo: probably place to a separate method
      // Render the scene to the second, helper framebuffer - another view of the plane
      glBindFramebuffer(GL_FRAMEBUFFER, mHelpFbo.mFramebuffer);
      // Setup the camera such that we look backwards
      mCamera->setRotate(glm::vec3(30.0f, 180.0f, 0.f));
      const glm::vec3 currentTranslate = mCamera->getTranslate();
      mCamera->setTranslate(glm::vec3(0.0f, 0.0f, -4.5f));
      mCamera->updateModelMatrix();
      drawScene();
      swapBuffers();

      // Restore the camera translation
      mCamera->setTranslate(currentTranslate);

      // Render the textures to the screen
      // Bind the default framebuffer and draw the texture containing the scene
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

      // Main scene
      if (mDepthBufferMode) // Display the depth buffer instead on the main scene
      {
         mMainFboQuad->getTexture()->setId(mMainFbo.mTexDepthBuffer);
         mMainFboQuad->setShadersAndBuffers(mDepthBufferShader);
      }
      draw(mMainFboQuad.get());

      // Small helper scene
      glViewport(0.7f*mWndWidth, 0, 0.3f*mWndWidth, 0.3f*mWndHeight);
      draw(mHelpFboQuad.get());

      glEnable(GL_DEPTH_TEST);

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
      CLog::getInstance().logGL("* Depth test enabled: ");

      glEnable(GL_TEXTURE_2D);
      CLog::getInstance().logGL("* Textures enabled: ");

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      CLog::getInstance().logGL("* Blending enabled, glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA): ");
      CLog::getInstance().log("");

      if (mWndWidth > 0 && mWndHeight > 0)
      {
         setupFbo(mMainFbo, mMainFboQuad, mFboShader, mWndWidth, mWndHeight);
         setupFbo(mHelpFbo, mHelpFboQuad, mFboShader, mWndWidth, mWndHeight);

         if (mHelpFboQuad)
            mHelpFboQuad->setTextureUnit(GL_TEXTURE1); // Set a different texture unit to use several textures simultaneously

         if (mDepthBufferShader)
            mDepthBufferShader->link();
      }

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
      {
         CLog::getInstance().log("* render context was destroyed");
      }
      else
      {
         CLog::getInstance().log("* ERROR: render context wasn't destroyed");
      }
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

void CWin32Renderer::drawScene()
{
   glClearColor(0.95f, 0.95f, 0.95f, 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // At first draw opaque objects
   /// @todo: put to a method with an array as an argument
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

   // Then draw transparent objects (they switch the depth off)
   for (auto * pRenderable : mTransparentRenderables)
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
}

void CWin32Renderer::init(const CWin32Window & window)
{
   mDC = window.getDC();
   mIsFullScreen = window.isFullScreen();
   mWndWidth = window.getWidth();
   mWndHeight = window.getHeight();
   mWndHandle = window.getWindowHandle();
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
      // Translate in xz plane
      ///@todo: use x, z components later
      glm::vec3 position = mAirplane->getPosition();

      const glm::vec3 direction = mAirplane->getDirectionOfFlight();
      position = position - direction*mAirplane->getSpeedOfFlight()*glm::vec3(mFrameDt, 0.0f, mFrameDt);

      // Periodic boundaries
      ///@todo: put to a method
      if (mLand && mSky)
      {
         const float halfLandZ = mLand->getScale().z * 0.5f;
         const float halfLandX = mLand->getScale().x * 0.5f;
         if (position.z > halfLandZ - 0.5f*mSky->getScale().z)
            position.z -= 2.f*halfLandZ - 0.5f*mSky->getScale().z;

         if (position.z < -halfLandZ + 0.5f*mSky->getScale().z)
            position.z += 2.f*halfLandZ + 0.5f*mSky->getScale().z;

         if (position.x > halfLandX - 0.5f*mSky->getScale().x)
            position.x -= 2.f*halfLandX - 0.5f*mSky->getScale().x;

         if (position.x < -halfLandX + 0.5f*mSky->getScale().x)
            position.x += 2.f*halfLandX + 0.5f*mSky->getScale().x;
      }

      mAirplane->setPosition(position);

      // New position of the center of the sky is the position of the airplane
      glm::vec3 skyPos = position;
      skyPos.y = 0.0f;

      if (mSky)
      {
         glm::mat4 skyMatrix = glm::mat4(1.0f);
         skyMatrix = glm::translate(skyMatrix, skyPos);
         mSky->updateTRMatrix(skyMatrix, mFrameDt);
         mSky->updateModelMatrix();
      }

      /// Translate clouds together with the plane and the sky
      for (auto cloud : mClouds)
      {
         if (cloud)
         {
            glm::mat4 cloudMatrix = glm::mat4(1.0f);
            cloudMatrix = glm::translate(cloudMatrix, skyPos + cloud->getPosition());
            cloud->updateTRMatrix(cloudMatrix, mFrameDt);
            cloud->updateModelMatrix();
         }
      }

      mAirplaneMatrix = glm::translate(mAirplaneMatrix, mAirplane->getPosition());

      // Rotate around y-axis
      glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
      float angleYradians = std::asin(direction.x);
      if (direction.z < 0.0f)
         angleYradians = static_cast<float>(M_PI) - angleYradians;
      mAirplaneMatrix = glm::rotate(mAirplaneMatrix, angleYradians, yAxis);
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
   ///@todo: place to a method updateSphere
   if (mSphereRoot)
   {
      mSphereRoot->updateTRMatrix(glm::mat4x4(1.0f), mFrameDt);
      mSphereRoot->updateModelMatrix();
      mSphereRoot->update(mFrameDt);
   }

   for (std::size_t count = 0; count < mStar.size(); ++count)
   {
      if (mStar[count])
      {
         mStar[count]->update(mFrameDt);
      }

      CAnimationBillBoard* star = mStar[count].get();
      if (star)
      {
         star->updateTRMatrix(glm::mat4x4(1.0f), mFrameDt);
         star->updateModelMatrix();
      }
   }

   if (mTurbineFire)
   {
      mTurbineFire->update(mFrameDt);
      mTurbineFire->updateTRMatrix(mTurbineFire->getTRMatrix(), mFrameDt);
   }

   if (mTurbineSmoke)
   {
      mTurbineSmoke->update(mFrameDt);
      mTurbineSmoke->updateTRMatrix(mTurbineSmoke->getTRMatrix(), mFrameDt);
   }

   updateAirplane();

   // Prevent billboards from rotations with the airplane
   if (mTurbineFire)
   {
      mTurbineFire->updateModelMatrix();
   }

   if (mTurbineSmoke)
   {
      mTurbineSmoke->updateModelMatrix();
   }

   if (mRightMissile && mRightMissile->isDetached())
   {
      mRightMissile->update(mFrameDt);
   }

   if (mExplosion)
   {
      mExplosion->update(mFrameDt);
      mExplosion->calculateModelMatrix();
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
   const float rotationSpeed = 100.f*mFrameDt;
   const float translateSpeed = 4.f*mFrameDt;
   if (mKeyPressed)
   {
      switch (mKeyCode)
      {
         /// Airplane rotations
      case (VK_LEFT) :
      {
         mAngleZ += rotationSpeed;
         mAngleZ = std::min<float>(mAngleZ, 50.f);
         mAirplane->rotateFlightDirection(1.0f, mFrameDt);
      }
         break;

      case (VK_RIGHT) :
      {
         mAngleZ -= rotationSpeed;
         mAngleZ = std::max<float>(mAngleZ, -50.f);
         mAirplane->rotateFlightDirection(-1.0f, mFrameDt);
      }
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

               ///@todo:make 2 members: original speed and current speed and call restore speed here to avoid magic numbers
               //mAirplane->setSpeedOfFlight(glm::vec3(3.0f, 18.0f, 3.0f));
               mAirplane->resetSpeedOfFlight();

               // Move upwards
               glm::vec3 position = mAirplane->getPosition();
               ///@todo: move to handle collisions
               position.y = std::min<float>(25.f, position.y + mAirplane->getSpeedOfFlight().y*mFrameDt);
               mAirplane->setPosition(position);

               if (mTurbineFire)
               {
                  mTurbineFire->setEmitSpeed(2.5f);
               }
               if (mTurbineSmoke)
               {
                  mTurbineSmoke->setEmitSpeed(2.5f);
               }
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
      //case (0x31) : // 1, debug mode on
      //   mIsDebugMode = true;
      //   break;

      //case (0x32) : // 2, debug mode off
      //   mIsDebugMode = false;
      //   break;

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

      case (0x55) : // u, accelerate
         if (mAirplane)
         {
            glm::vec3 currentSpeed = mAirplane->getSpeedOfFlight();
            currentSpeed.z += 1.0f;
            currentSpeed.z = std::min<float>(currentSpeed.z, 30.f);
            currentSpeed.x += 1.0f;
            currentSpeed.x = std::min<float>(currentSpeed.x, 30.f);
            mAirplane->setSpeedOfFlight(currentSpeed);
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
      const CBoundingBox* boxLand = 0; // mLand->getBoundingBox();
      if (boxAirPlane && boxLand)
      {
         if (boxAirPlane->collidesWith(*boxLand))
         {
            mAirplane->resetHealthBars();
            mAirplane->setPropellerSpeed(0.0f);
            mAirplane->setSpeedOfFlight(glm::vec3(0.0f, 0.0f, 0.0f));
            // Restore the previous position of the plane
            glm::vec3 newPos = mAirplane->getPosition();
            newPos.y = newPos.y + mAirplane->getSpeedOfFlight().y*mFrameDt;
            mAirplane->setPosition(newPos);
            if (mTurbineFire)
            {
               mTurbineFire->setEmitSpeed(0.0f);
            }
            if (mTurbineSmoke)
            {
               mTurbineSmoke->setEmitSpeed(0.0f);
            }
         }
      }
   }

   /// Airplane vs stars collisions
   ///@todo: put to a separate method
   if (mAirplane)
   {
      for (std::size_t count = 0; count < mStar.size(); ++count)
      {
         const CBoundingBox* boxAirPlane = mAirplane->getBoundingBox();
         const CBoundingBox* boxStar = mStar[count]->getBoundingBox();
         if (boxAirPlane && boxStar && boxStar->isVisible())
         {
            if (boxAirPlane->collidesWith(*boxStar, false))
            {
               float health = mAirplane->getHealth();
               mAirplane->resetHealthBars(std::min<float>(health + 0.15f, 1.0f));
               mStar[count]->setVisible(false);
            }
         }
      }
   }

   ///@todo: add bounding box to the missile later
   // Missiles and land
   if (mLand && mRightMissile && mRightMissile->isDetached() && mAirplaneRoot)
   {
      glm::vec3 currentPos = mRightMissile->getTranslate();
      if (currentPos.y <= mLand->getTranslate().y)
      {
         if (mExplosion)
         {
            const glm::vec3 explosionPos = glm::vec3(currentPos.x, mLand->getTranslate().y + 1.0f, currentPos.z);
            mExplosion->setTranslate(explosionPos);
            mExplosion->setVisible(true);
         }
         mRightMissile->setDetached(false);
         currentPos = glm::vec3(2.0f, -1.0f, 1.0f);///@todo: do not use magic numbers

         mRightMissile->setRotate(glm::vec3(0.0f, 0.0f, 0.0f));
         mRightMissile->setTranslate(currentPos);

         mRightMissile->buildModelMatrix(mAirplaneRoot->getTRMatrix());
         mAirplaneRoot->add(mRightMissile.get());
         mRightMissile->setFireVisible(false);
      }
   }
}

void CWin32Renderer::setupFbo(SFrameBuffer& fbo, std::unique_ptr<CQuad>& quad, std::shared_ptr<CShader>& shader,
                              const GLint width, const GLint height)
{
   glGenFramebuffers(1, &fbo.mFramebuffer);
   glBindFramebuffer(GL_FRAMEBUFFER, fbo.mFramebuffer);

   // Create a color attachment texture
   generateAttachmentTexture(fbo);

   // Attach color and depth textures
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.mTexColorBuffer, 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo.mTexDepthBuffer, 0);

   // Set the list of draw buffers.
   GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
   glDrawBuffers(sizeof(drawBuffers) / sizeof(drawBuffers[0]), drawBuffers); // "2" is the size of DrawBuffers

   if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      CLog::getInstance().log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   if (shader && quad)
   {
      shader->link();
      quad->setShadersAndBuffers(shader);
      quad->getTexture()->setId(fbo.mTexColorBuffer);
   }
}

void CWin32Renderer::generateAttachmentTexture(SFrameBuffer& fbo)
{
   // What enum to use?
   GLenum attachment_type = GL_RGB;

   //Generate texture ID and load texture data for the color buffer
   glGenTextures(1, &fbo.mTexColorBuffer);
   glBindTexture(GL_TEXTURE_2D, fbo.mTexColorBuffer);

   glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, mWndWidth, mWndHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glBindTexture(GL_TEXTURE_2D, 0);

   // Generate the texture for the depth buffer
   glGenTextures(1, &fbo.mTexDepthBuffer);
   glBindTexture(GL_TEXTURE_2D, fbo.mTexDepthBuffer);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, mWndWidth, mWndHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glBindTexture(GL_TEXTURE_2D, 0);
}

///@todo: remove this method when all the game-related stuff is moved to CGame
bool CWin32Renderer::windowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
   switch (uMessage)
   {
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

      case (0x37) : // 7, show all stars
         CLog::getInstance().log("* Button 7: Show all stars");
         for (auto star : mStar)
         {
            if (star)
               star->setVisible(true);
         }
         break;

         ///@todo: remove commented
      //case (0x38) : // 8 display the depth buffer
      //   mDepthBufferMode = !mDepthBufferMode;
      //   CLog::getInstance().log("* Button 8: display depth buffer: ", mDepthBufferMode);
      //   break;

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

      case (VK_SPACE) : // Space, shot the missile
         if (!mThirdKeyPressed)
         {
            mAirplaneRoot->remove(mRightMissile.get());
            mRightMissile->setDetached(true);

            // Set the position in the world space and all actions are in the world space
            glm::vec3 positionWorld = mAirplane->getPosition() + glm::vec3(0.0f, 0.0f, 0.0f);
            mRightMissile->setTranslate(positionWorld);
            mRightMissile->setFlightDirection(mAirplane->getFlightDirection());
            //mRightMissile->setFireVisible(true);
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

      if (mTurbineFire && ((wParam == VK_SPACE) || (wParam == VK_UP)))
      {
         mTurbineFire->resetEmitSpeed();
      }
      if (mTurbineSmoke && ((wParam == VK_SPACE) || (wParam == VK_UP)))
      {
         mTurbineSmoke->resetEmitSpeed();
      }

      if ((wParam == 0x55) && mAirplane)
      {
         mAirplane->resetSpeedOfFlight();
      }
   }
   return false;
   } // end switch

   // Process other messages
   return true;
}

void CWin32Renderer::setAirplaneRoot(CParentGameObject * root)
{
   if (root)
   {
      // By default, attach the camera to the plane
      mAirplaneRoot = root;
      mAirplaneRoot->add(mCamera.get());
      mAirplaneRoot->buildModelMatrix(glm::mat4x4(1.0f));
      mCameraAttached = true;
      CLog::getInstance().log("* Button Enter: Camera attached to the plane");
   }
}

void CWin32Renderer::setupEvents()
{
   bool status = false;

   // Debug mode on/off
   status = CEventManager::getInstance().registerEvent(CGame::DEBUG_MODE_EVENT);
   CLog::getInstance().logGL("DEBUG_MODE_EVENT attached: ", status);
   CEventManager::getInstance().attachEvent(CGame::DEBUG_MODE_EVENT, *this);

   // Depth buffer on/off
   status = CEventManager::getInstance().registerEvent(CGame::DEPTHBUF_EVENT);
   CLog::getInstance().logGL("DEPTHBUF_EVENT attached: ", status);
   CEventManager::getInstance().attachEvent(CGame::DEPTHBUF_EVENT, *this);
}

void CWin32Renderer::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      switch (pEvent->getId())
      {
      case CGame::DEBUG_MODE_EVENT:
         mIsDebugMode = !mIsDebugMode;
         CLog::getInstance().log("* Debug mode on: ", mIsDebugMode);
         break;

      case CGame::DEPTHBUF_EVENT:
         mDepthBufferMode = !mDepthBufferMode;
         CLog::getInstance().log("* Display depth buffer: ", mDepthBufferMode);
         break;
      }
   } // End if
}

