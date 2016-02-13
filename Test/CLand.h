#ifndef AERO_SIMULATOR_CLAND_H
#define AERO_SIMULATOR_CLAND_H

#include "../AeroSimulator/CGameObject.h"
#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/glm/vec2.hpp"

namespace AeroSimulatorEngine
{
   class CShader;
   class CRenderable;

   class CLand : public CGameObject
   {
   public:
      CLand(const int id,
            const int type,
            std::shared_ptr<CShader>& pShader,
            const char * textureFilePath,
            const glm::vec2& numOfTiles);

      virtual ~CLand();

      CRenderable & CLand::getRenderable();

   private:
      void scaleVertices(GLfloat* vertices, const int numOfVertices) const;

      void addRenderableComponent(std::shared_ptr<CShader>& pShader, const char * textureFilePath);
      void addTransformComponent();
      void addMovementComponent();

   private:
      glm::vec2 mNumOfTiles;
   };
} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLAND_H
