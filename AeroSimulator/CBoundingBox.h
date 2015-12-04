#ifndef AERO_SIMULATOR_CBOUNDING_BOX_H
#define AERO_SIMULATOR_CBOUNDING_BOX_H

#include "CCube.h"

namespace AeroSimulatorEngine
{
   class CBoundingBox : public CCube
   {
   public:
      CBoundingBox();
      virtual ~CBoundingBox();
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CBOUNDING_BOX_H
