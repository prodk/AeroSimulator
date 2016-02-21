#ifndef AERO_SIMULATOR_CUTILS_H
#define AERO_SIMULATOR_CUTILS_H

#include <vector>
#include "../AeroSimulator/include/glew.h"
#include "glm/gtc/constants.hpp"

#define ARRAYLEN(a) sizeof((a)) / sizeof((a[0]))

namespace AeroSimulatorEngine
{
   struct SSphereParams
   {
      SSphereParams(float radius, std::size_t numOfCircles, std::size_t numOfSegments, float maxInclination, float maxAzimuth);

      float mRradius;
      std::size_t mNumOfCircles;
      std::size_t mNumOfSegments;
      float mMaxInclination;
      float mMaxAzimuth;
   };

   class CUtils
   {
   public:
      CUtils();
      ~CUtils();

      static
      void generateTexturedSphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, const SSphereParams& params);
   };
}

#endif // AERO_SIMULATOR_CUTILS_H
