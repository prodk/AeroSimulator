#ifndef AERO_SIMULATOR_CLAND_H
#define AERO_SIMULATOR_CLAND_H

#include "CParentGameObject.h"

///@todo: move CLand and other game objects to Test project

namespace AeroSimulatorEngine
{
   class CBoundingBox;

   class CLand : public CParentGameObject
   {
   public:
      CLand();
      virtual ~CLand();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader); /// Requires a valid render context

      bool loadTexture(const char * fileName);

      /// Composite-related methods - override only some of them
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix) override;
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt) override;
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f)) override;

      void setNumOfTiles(const GLint x, const GLint y);

   private:
      glm::mat4x4 mScaledTRMatrix;
      glm::vec2 mNumOfTiles;
   };
} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLAND_H
