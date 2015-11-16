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

      bool windowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

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
      float mOldAngleZ;
      float mAngleX;
      std::size_t mHorizontalPressed;
      std::size_t mVerticalPressed;

      ///@todo: probably create an array of cameras later
      std::shared_ptr<CCamera> mCamera;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CWIN32_RENDERER_H
