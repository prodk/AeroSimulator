// CRenderer.h - declaration of an abstract CRenderer class

#ifndef AERO_SIMULATOR_CRENDERER_H
#define AERO_SIMULATOR_CRENDERER_H

#include "CTask.h"
#include "CEventHandler.h"

#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/wglew.h"
#include <gl/GL.h>
#include "../AeroSimulator/include/glext.h"

#include <vector>

namespace AeroSimulatorEngine
{
   class CRenderable; // Forward declarations should be inside the same namespace

   class CRenderer : public CTask, public CEventHandler
   {
   public:
      explicit CRenderer(ePriority prio);
      virtual ~CRenderer();

      void addRenderable(CRenderable* pRenderable);
      void removeRenderable(CRenderable* pRenderable);

      bool loadOpenGLExtensions();

   protected:
      // Methods to override
      virtual void init() = 0;
      virtual void destroy() = 0;
      virtual void draw(CRenderable* pRenderable) = 0;
      virtual void swapBuffers() = 0;

   protected:
      typedef std::vector<CRenderable*> RenderableVector;
      RenderableVector mRenderables;
      RenderableVector mTransparentRenderables;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERER_H
