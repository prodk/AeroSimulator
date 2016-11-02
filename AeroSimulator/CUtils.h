#ifndef AERO_SIMULATOR_CUTILS_H
#define AERO_SIMULATOR_CUTILS_H

#include <vector>
#include <memory>
#include <map>
#include <string>
#include "../AeroSimulator/include/glew.h"
#include "glm/gtc/constants.hpp"
#include "../AeroSimulator/include/glm/mat4x4.hpp"

#define ARRAYLEN(a) sizeof((a)) / sizeof((a[0]))

namespace AeroSimulatorEngine
{
   class CTransform;
   class CShader;
   class CGameObject;

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

      static void generateTexturedSphere(std::vector<GLfloat>& vertices,
                                         std::vector<GLuint>& indices,
                                         const SSphereParams& params);

      static void addColorCube(const CTransform & transform,
                               const glm::vec4 & color,
                               const int objectType,
                               std::shared_ptr<CShader>& shader,
                               std::map<int, std::shared_ptr<CGameObject>> & children,
                               const std::string& msg);
   };
}

#endif // AERO_SIMULATOR_CUTILS_H
