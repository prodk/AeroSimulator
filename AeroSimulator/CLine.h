#ifndef AERO_SIMULATOR_CLINE_H
#define AERO_SIMULATOR_CLINE_H

#include "CLeafGameObject.h"
#include <memory>

namespace AeroSimulatorEngine
{
   class CLine : public CLeafGameObject
   {
   public:
      CLine();
      virtual ~CLine();
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLINE_H
