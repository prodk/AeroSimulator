// CRenderer.h - declaration of an abstract CRenderer class

#ifndef AERO_SIMULATOR_CRENDERER_H
#define AERO_SIMULATOR_CRENDERER_H

#include "CTask.h"

namespace AeroSimulatorEngine
{
   // TODO: make it a singleton, only one CRenderer can exist
class CRenderer : public CTask
{
public:
   explicit CRenderer(ePriority prio);
   virtual ~CRenderer();

   // Methods to override
   virtual void init() = 0;
   virtual void destroy() = 0;
   virtual void draw() = 0;
   virtual void swapBuffers() = 0;
};

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERER_H
