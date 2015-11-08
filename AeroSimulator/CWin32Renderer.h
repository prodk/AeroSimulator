// CWin32Renderer.h - declaration of the concrete Renderer for the Windows platform

#ifndef AERO_SIMULATOR_CWIN32_RENDERER_H
#define AERO_SIMULATOR_CWIN32_RENDERER_H

#include "CRenderer.h"
#include "CWin32Window.h"

namespace AeroSimulatorEngine
{
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

   private:
      // Override CRenderer part
      virtual void init();
      virtual void destroy();
      virtual void draw(CRenderable* pRenderable);
      virtual void swapBuffers();

   private:
      bool createRenderContext();
      bool loadOpenGLExtensions(); // TODO: may be redundant when GLEW is used

   private:
      HDC mDC;
      bool mIsInitialized;
      HGLRC mRenderContext;
      HGLRC mOldRenderContext;
      bool mIsFullScreen;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CWIN32_RENDERER_H
