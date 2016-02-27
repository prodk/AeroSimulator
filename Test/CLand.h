#ifndef AERO_SIMULATOR_CLAND_H
#define AERO_SIMULATOR_CLAND_H

#include "../AeroSimulator/CGameObject.h"
#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/glm/vec2.hpp"
#include "../AeroSimulator/include/glm/vec3.hpp"

#include <string>

namespace AeroSimulatorEngine
{
   class CShader;
   class CRenderable;
   struct SRenderableData;

   class CLand : public CGameObject
   {
   public:
      CLand(const int id,
            const int type,
            SRenderableData& data,
            const glm::vec2& numOfTiles,
            const glm::vec3& size);

      virtual ~CLand();

   private:
      void scaleVertices(GLfloat* vertices, const int numOfVertices) const;
      void addTransformComponent(const glm::vec3& size);

   private:
      glm::vec2 mNumOfTiles;
   };
} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLAND_H
