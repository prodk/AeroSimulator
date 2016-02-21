#include "CUtils.h"
#include "../AeroSimulator/include/glm/vec3.hpp"
#include "../AeroSimulator/include/glm/mat4x4.hpp"
#include "../AeroSimulator/include/glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

SSphereParams::SSphereParams(float radius, std::size_t numOfCircles, std::size_t numOfSegments, float maxInclination, float maxAzimuth)
   : mRradius(radius)
   , mNumOfCircles(numOfCircles)
   , mNumOfSegments(numOfSegments)
   , mMaxInclination(maxInclination)
   , mMaxAzimuth(maxAzimuth)
{
}

CUtils::CUtils()
{
}

CUtils::~CUtils()
{
}

void CUtils::generateTexturedSphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, const SSphereParams& params)
{
   if ((params.mNumOfCircles > 0) && (params.mNumOfSegments > 0) &&
       (params.mRradius > 0.0f) && (params.mMaxInclination > 0.0f) && (params.mMaxAzimuth > 0.0f))
   {
      // The generated sphere is z-oriented. We need to rotate it around x by pi/2 to get it y-oriented
      const float angleX = glm::radians(-90.0f);
      const glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
      const glm::mat4 rotateMatrix = glm::rotate(glm::mat4(), angleX, xAxis);

      // Vertices
      const float r = params.mRradius;
      const float deltaTheta = params.mMaxInclination / static_cast<float>(params.mNumOfCircles);
      const float deltaPhi = params.mMaxAzimuth / static_cast<float>(params.mNumOfSegments);

      float theta = 0.0f;
      for (GLuint i = 0; i < params.mNumOfCircles + 1; ++i) ///@note +1
      {
         float phi = 0.0f;
         for (GLuint j = 0; j < params.mNumOfSegments + 1; ++j)
         {
            glm::vec3 pos;
            pos.x = std::sin(theta) * std::cos(phi);
            pos.y = std::sin(theta) * std::sin(phi);
            pos.z = std::cos(theta);

            // For texture just set the usual coords
            const float textureX = phi / params.mMaxAzimuth;
            const float textureY = theta / params.mMaxInclination;

            // Texture coordinates
           /* const float pi = glm::pi<float>();
            const float textureY = std::acos(pos.z / r) / (pi);

            const float posxAdjust = (pos.x < std::numeric_limits<float>::epsilon()) ?
               std::numeric_limits<float>::epsilon() :
               pos.x;
            const float textureX = (std::atan(pos.y / posxAdjust) + pi) / (2.0f*pi);*/

            // Rotate around x to make it y-oriented
            pos = glm::mat3(rotateMatrix) * pos;

            vertices.push_back(r * pos.x);
            vertices.push_back(r * pos.y);
            vertices.push_back(r * pos.z);
            vertices.push_back(textureX);
            vertices.push_back(textureY);

            phi += deltaPhi;
         }
         theta += deltaTheta;
      }

      // Indices, the following pattern is generated (for 4 segments):
      // stripe 0: 0415263704; stripe 1: 48596 10 7 11 4 8
      for (GLuint i = 1; i < params.mNumOfCircles + 1; ++i)
      {
         for (GLuint j = 0; j < params.mNumOfSegments + 1; ++j)
         {
            std::size_t id = (i - 1) * (params.mNumOfSegments + 1) + j;
            indices.push_back(id);
            id = i * (params.mNumOfSegments + 1) + j;
            //if (id < (params.mNumOfCircles + 1) * (params.mNumOfSegments + 1))
            {
               indices.push_back(id);
            }

            /*if (params.mNumOfSegments == j)
            {
               indices.push_back((i - 1) * (params.mNumOfSegments + 1));
               indices.push_back(i * (params.mNumOfSegments + 1));
            }*/
         }
      }
   }
}
