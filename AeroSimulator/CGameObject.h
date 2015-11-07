#ifndef AERO_SIMULATOR_CGAME_OBJECT_H
#define AERO_SIMULATOR_CGAME_OBJECT_H

#include "CRenderable.h"

namespace AeroSimulatorEngine
{
   class CGameObject : public CRenderable
   {
   public:
      CGameObject();
      virtual ~CGameObject();

      virtual void setupGeometry() = 0;
      virtual void setupVBO() = 0;
   };

} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CGAME_OBJECT_H
