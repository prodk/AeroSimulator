#ifndef AERO_SIMULATOR_CCOMMON_MATH_H
#define AERO_SIMULATOR_CCOMMON_MATH_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "glm/mat4x4.hpp"
#include "glm/mat3x3.hpp"

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

      static float radToDeg(const float angleRadians)
      {
         return angleRadians * hundredEightyToPi;
      }

      static void copyColumn(int columnId, glm::mat3x3 & out, const glm::mat4x4 in)
      {
         out[columnId].x = in[columnId].x;
         out[columnId].y = in[columnId].y;
         out[columnId].z = in[columnId].z;
      }

      static const float piTo180;
      static const float hundredEightyToPi;
   };

} // namespace AeroSimulatorEngine
#endif //AERO_SIMULATOR_CCOMMON_MATH_H
