#ifndef AERO_SIMULATOR_CPROPELLER_OBJECT_H
#define AERO_SIMULATOR_CPROPELLER_OBJECT_H

#include "CParentGameObject.h"

namespace AeroSimulatorEngine
{
   class CPropeller : public CParentGameObject
   {
   public:
      CPropeller();
      virtual ~CPropeller();

      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CPROPELLER_OBJECT_H
