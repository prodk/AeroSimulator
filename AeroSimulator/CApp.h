// CApp.h - the application class, it is a singleton

#ifndef AERO_SIMULATOR_CAPP_H
#define AERO_SIMULATOR_CAPP_H

#include "CTaskManager.h"

#include <iostream>
#include <memory>
#include <vector>

#define APP CApp::getInstance()

///@todo: rename to NAeroSimulatorEngine
namespace AeroSimulatorEngine
{
   class CWin32Window;
   class CWin32Renderer;
   class CRenderer;
   class CTimer;

   class CApp
   {
   public:
      static CApp& getInstance()
      {
         static CApp instance;
         return instance;
      }

      ~CApp();

      ///@todo: later add passing app parameters read from a config file here
      bool init(const char* name, unsigned int width, unsigned int height);
      void run();
      int exit();

      void addTask(CTask *pTask);

      ///@todo: think how to avoid exposing a private member
      CRenderer* getRenderer() const;

   private:
      CApp();
      // Disable the copy constructor and copy assignment operator
      CApp(const CApp&) = delete;
      void operator=(const CApp&) = delete;

      ///@todo: all these methods move to CGame
      /* void addClouds();
      void addSphere();
      void addStars();
      void addFire();
      void addSmoke();
      void addMissiles();*/

   private:

      ///@todo: introduce a Bridge pattern and place Win32-specific code there
      CTaskManager mTaskManager;
      std::shared_ptr<CWin32Window> mAppWindowTask;
      std::shared_ptr<CWin32Renderer> mRendererTask;
      std::shared_ptr<CTimer> mTimerTask;

      /*
      std::shared_ptr<CShader> mTextureShader;
      std::shared_ptr<CShader> mBillboardShader;
      std::shared_ptr<CShader> mColorShader;
      std::shared_ptr<CShader> mHealthbarShader;
      std::shared_ptr<CShader> mColorLambertianShader;
      std::shared_ptr<CShader> mAnimationBbShader;
      std::shared_ptr<CShader> mNormalMapSphereShader;*/

      ///@todo: place billboards to some bridge class CClouds
      //std::vector<std::shared_ptr<CBillBoard> > mBillBoards; ///@todo: rename into mClouds
      //std::shared_ptr<CAnimationBillBoard> mStar;
      //std::vector<std::shared_ptr<CAnimationBillBoard> > mStar;

      //std::shared_ptr<CParticleSystem> mTurbineFire;
      //std::shared_ptr<CParticleSystem> mTurbineSmoke;
      //std::shared_ptr<CMissile> mRightMissile;
      //std::shared_ptr<CAnimationBillBoard> mExplosion;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPP_H
