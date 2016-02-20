#ifndef AERO_SIMULATOR_CLAND_H
#define AERO_SIMULATOR_CLAND_H

#include "../AeroSimulator/CGameObject.h"
#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/glm/vec2.hpp"
#include "../AeroSimulator/include/glm/vec3.hpp"

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
            const glm::vec2& numOfTiles,
            const glm::vec3& size);

      virtual ~CLand();

   private:
      void scaleVertices(GLfloat* vertices, const int numOfVertices) const;

      ///@todo: make addComponent virtual such that no need to declare them in every class
      void addRenderableComponent(std::shared_ptr<CShader>& pShader, const char * textureFilePath);
      void addTransformComponent(const glm::vec3& size);
      void addMovementComponent();

      CRenderable & getRenderable();

   private:
      glm::vec2 mNumOfTiles;
   };
} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLAND_H
