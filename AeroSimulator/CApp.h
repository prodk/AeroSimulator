// CApp.h - the application class, it is a singleton

#ifndef AERO_SIMULATOR_CAPP_H
#define AERO_SIMULATOR_CAPP_H

#include "CTaskManager.h"

#include <iostream>
#include <memory>
#include <vector>

namespace AeroSimulatorEngine
{
   class CWin32Window;
   class CWin32Renderer;
   class CShader;
   class CGeometry;
   class C3DModel;
   class CTextureShader;
   class CGameObject;
   class CCompositeGameObject;

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

   private:
      CApp();
      // Disable the copy constructor and copy assignment operator
      CApp(const CApp&) = delete;
      void operator=(const CApp&) = delete;

      void setupScene();
      void addSkyBox();
      void addLand();
      void addAirplane();
      void addBillboards();
      void addSphere();

   private:
      typedef std::shared_ptr<CGameObject> tGameObjectPtr;

      ///@todo: introduce a Bridge pattern and place Win32-specific code there
      CTaskManager mTaskManager;
      std::shared_ptr<CWin32Window> mAppWindowTask;
      std::shared_ptr<CWin32Renderer> mRendererTask;

      std::shared_ptr<C3DModel> mAirPlane;
      std::shared_ptr<CShader> mSimpleShader; ///@todo: probably remove this
      std::shared_ptr<CShader> mTextureShader;
      std::shared_ptr<CShader> mBillboardShader;
      std::shared_ptr<CShader> mColorShader;
      std::shared_ptr<CShader> mColorBillboardShader;
      std::shared_ptr<CShader> mColorLambertianShader;
      tGameObjectPtr mSkyBox;
      tGameObjectPtr mLand;
      std::shared_ptr<CCompositeGameObject> mSphere;

      ///@todo: place billboards to some bridge class CClouds
      std::vector<tGameObjectPtr > mBillBoards;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPP_H
