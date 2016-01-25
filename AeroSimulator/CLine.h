#ifndef AERO_SIMULATOR_CLINE_H
#define AERO_SIMULATOR_CLINE_H

//#include "CLeafGameObject.h"
#include "CGameObject.h"
#include <memory>

namespace AeroSimulatorEngine
{
   class CLine : public CGameObject
   {
   public:
      CLine();
      virtual ~CLine();
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLINE_H
