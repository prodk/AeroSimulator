// CRenderer.h - declaration of an abstract CRenderer class
//#pragma once

#ifndef AERO_SIMULATOR_CRENDERER_H
#define AERO_SIMULATOR_CRENDERER_H

#include "CTask.h"

#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/wglew.h"
#include <gl/GL.h>
#include "../AeroSimulator/include/glext.h"

#include <vector>

namespace AeroSimulatorEngine
{
   class CRenderable; // Forward declarations should be inside the same namespace

   // TODO: make it a singleton, only one CRenderer can exist
   class CRenderer : public CTask
   {
   public:
      explicit CRenderer(ePriority prio);
      virtual ~CRenderer();

      void addRenderable(CRenderable* pRenderable);
      void removeRenderable(CRenderable* pRenderable);

   protected:
      // Methods to override
      virtual void init() = 0;
      virtual void destroy() = 0;
      virtual void draw(CRenderable* pRenderable) = 0;
      virtual void swapBuffers() = 0;

   protected:
      typedef std::vector<CRenderable*> RenderableVector;
      RenderableVector mRenderables;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERER_H
