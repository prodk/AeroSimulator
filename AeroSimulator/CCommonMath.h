#ifndef AERO_SIMULATOR_CCOMMON_MATH_H
#define AERO_SIMULATOR_CCOMMON_MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

namespace AeroSimulatorEngine
{
   class CCommonMath
   {
   public:
      CCommonMath();
      ~CCommonMath();

      static float degToRad(const float angleDegrees)
      {
         return angleDegrees * piTo180;
      }

      static const float piTo180;
   };

} // namespace AeroSimulatorEngine
#endif //AERO_SIMULATOR_CCOMMON_MATH_H
