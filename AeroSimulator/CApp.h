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
  // class CSkyBox;

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

      void setupRenderer();///@todo: rename this method as we also build the tree here, setup scene or smth like that
      void addSkyBox();
      void addLand();
      void addAirplane();
      void addBillboards();

   private:
      typedef std::shared_ptr<CGameObject> tGameObjectPtr;

      ///@todo: introduce a Bridge pattern and place Win32-specific code there
      CTaskManager mTaskManager;
      std::shared_ptr<CWin32Window> mAppWindowTask;
      std::shared_ptr<CWin32Renderer> mRendererTask;

      std::shared_ptr<C3DModel> mAirPlane;
      std::shared_ptr<CShader> mSimpleShader;
      std::shared_ptr<CShader> mTextureShader;
      std::shared_ptr<CShader> mBillboardShader;
      tGameObjectPtr mSkyBox;
      tGameObjectPtr mLand;

      ///@todo: place billboards to some bridge class CClouds
      std::vector<tGameObjectPtr > mBillBoards;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPP_H
