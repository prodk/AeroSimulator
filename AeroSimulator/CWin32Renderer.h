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

      void setAirplaneRoot(CCompositeGameObject* root) { if (root) mAirplaneRoot = root; };
      void setSphereRoot(CCompositeGameObject* root) { if (root) mSphereRoot = root; };

      //CCamera getCamera() const { return *mCamera; }

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

      void updateInput();

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

      CCompositeGameObject* mAirplaneRoot;
      CCompositeGameObject* mSphereRoot;

      bool mIsDebugMode;
      float mCameraScale;
      HWND mWndHandle;

      bool mKeyPressed;
      bool mCameraKeyPressed;
      WPARAM mKeyCode;
      WPARAM mCameraKeyCode;
      double mFrameDt;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CWIN32_RENDERER_H
