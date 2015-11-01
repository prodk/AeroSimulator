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
      virtual bool update(); // TODO: probably make it void
      virtual bool stop(); // TODO: probably make it void

      bool isInitialized() const { return mIsInitialized; }

      void init(const CWin32Window& window);

   private:
      // Override CRenderer part
      virtual void init();
      virtual void destroy();
      virtual void draw(CRenderable* pRenderable);
      virtual void swapBuffers();

   private:
      bool createRenderContext();
      bool setRenderContext();
      void resetRenderContext();
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
