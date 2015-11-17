// CWin32Renderer.h - declaration of the concrete Renderer for the Windows platform

#ifndef AERO_SIMULATOR_CWIN32_RENDERER_H
#define AERO_SIMULATOR_CWIN32_RENDERER_H

#include "CRenderer.h"
#include "CWin32Window.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <memory>

namespace AeroSimulatorEngine
{
   class CCamera;
   class CGameObject;

   class CWin32Renderer : public CRenderer
   {
   public:
      CWin32Renderer(ePriority prio);
      virtual ~CWin32Renderer();

      // Override CTask part
      virtual bool start();
      virtual void update();
      virtual void stop();

      bool isInitialized() const { return mIsInitialized; }

      void init(const CWin32Window& window);

      bool setRenderContext();
      void resetRenderContext();

      bool windowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

      void setRoot(CGameObject* root) { mRoot = root; };

   private:
      // Override CRenderer part
      virtual void init();
      virtual void destroy();
      virtual void draw(CRenderable* pRenderable);
      virtual void swapBuffers();

   private:
      bool createRenderContext();
      bool loadOpenGLExtensions();
      void calculateAirplaneMatrix(glm::mat4& matrix);

   private:
      HDC mDC;
      bool mIsInitialized;
      HGLRC mRenderContext;
      HGLRC mOldRenderContext;
      bool mIsFullScreen;

      float mAngleZ; // Angle of rotating around the z-axis of the air plane
      float mAngleX;
      std::size_t mHorizontalPressed;
      std::size_t mVerticalPressed;

      float mCameraAngleX; // up 'w', down 's'
      float mCameraAngleY; // left 'a', right 'd'

      ///@todo: probably create an array of cameras later
      std::shared_ptr<CCamera> mCamera;
      std::size_t mCameraVerticalPressed;
      std::size_t mCameraHorizontalPressed;

      CGameObject* mRoot;
      glm::mat4 mDynamicMatrix;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CWIN32_RENDERER_H
