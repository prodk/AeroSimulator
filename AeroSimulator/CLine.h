#ifndef AERO_SIMULATOR_CLINE_H
#define AERO_SIMULATOR_CLINE_H

#include "CLeafCompositeGameObject.h"
#include <memory>

namespace AeroSimulatorEngine
{
   class CLine : public CLeafCompositeGameObject
   {
   public:
      CLine();
      virtual ~CLine();
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLINE_H
