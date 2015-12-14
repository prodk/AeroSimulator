// CWin32Renderer.h - declaration of the concrete Renderer for the Windows platform

#ifndef AERO_SIMULATOR_CWIN32_RENDERER_H
#define AERO_SIMULATOR_CWIN32_RENDERER_H

#include "CRenderer.h"
#include "CWin32Window.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <memory>
#include <vector>

namespace AeroSimulatorEngine
{
   class CCamera;
   class CCompositeGameObject;
   class CParentGameObject;
   class C3DModel;
   class CAnimationBillBoard;
   class CLand;
   class CSkyBox;
   class CBillBoard;

   class CWin32Renderer : public CRenderer
   {
   public:
      CWin32Renderer(ePriority prio);
      virtual ~CWin32Renderer();

      // Override CTask part
      virtual bool start();
      virtual void update(CTask* pTask);
      virtual void stop();

      bool isInitialized() const { return mIsInitialized; }

      void init(const CWin32Window& window);

      bool setRenderContext();
      void resetRenderContext();

      bool windowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

      void setAirplaneRoot(CParentGameObject* root);
      void setSphereRoot(CParentGameObject* root) { if (root) mSphereRoot = root; }

      void setAirplane(std::shared_ptr<C3DModel>& airplane) { mAirplane = airplane; }
      void setStars(std::shared_ptr<CAnimationBillBoard>& stars) { if(stars) mStar.push_back(stars); }
      void setLand(std::shared_ptr<CLand>& land) { mLand = land; }
      void setSky(std::shared_ptr<CSkyBox>& sky) { mSky = sky; }

      void setClouds(const std::vector<std::shared_ptr<CBillBoard> >& clouds) { mClouds = clouds; }

   private:
      // Override CRenderer part
      virtual void init();
      virtual void destroy();
      virtual void draw(CRenderable* pRenderable);
      virtual void swapBuffers();

   private:
      bool createRenderContext();
      void updateAirplane();
      void updateCamera();
      void springButtons();
      void updateRenderables();
      void updateFPS(CTask* pTask);

      void updateInput(); ///@todo: move to CWin32Window or a separate handler when events are implemented
      void handleCollisions(); ///@todo; move to a special task when events are implemented

   private:
      HDC mDC;
      bool mIsInitialized;
      HGLRC mRenderContext;
      HGLRC mOldRenderContext;
      bool mIsFullScreen;

      ///@todo: reconsider the approach to spring buttons
      float mAngleZ; // Angle of rotating around the z-axis of the air plane
      float mAngleX;

      float mCameraAngleX; // up 'w', down 's'
      float mCameraAngleY; // left 'a', right 'd'

      ///@todo: probably create an array of cameras later
      std::shared_ptr<CCamera> mCamera;

      CParentGameObject* mAirplaneRoot;
      CParentGameObject* mSphereRoot;

      bool mIsDebugMode;
      bool mIsSetCameraMode;
      bool mCameraAttached;
      HWND mWndHandle;

      bool mKeyPressed;
      bool mCameraKeyPressed;
      bool mThirdKeyPressed;
      WPARAM mKeyCode;
      WPARAM mCameraKeyCode;
      WPARAM mThirdKeyCode;
      double mFrameDt;

      ///@todo: when the event framwork is setup, 
      ///@todo: transfer moving objects and collisions to a separaete task
      glm::mat4x4 mAirplaneMatrix;

      ///@todo: move to the movement/collisions task when events are implemented
      std::shared_ptr<C3DModel> mAirplane;
      std::vector<std::shared_ptr<CAnimationBillBoard> > mStar;
      std::shared_ptr<CLand> mLand;
      std::shared_ptr<CSkyBox> mSky;
      std::vector<std::shared_ptr<CBillBoard> > mClouds;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CWIN32_RENDERER_H
