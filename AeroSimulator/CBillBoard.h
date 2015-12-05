#ifndef AERO_SIMULATOR_CBILLBOARD_H
#define AERO_SIMULATOR_CBILLBOARD_H

#include "CParentGameObject.h"

namespace AeroSimulatorEngine
{
   class CBoundingBox;

   ///@todo: probably change CParentGameObject taking into account that it can have model matrix to avoid overriding methods
   ///@todo: then change CCube, CLand, CBillboard, CSphere
   class CBillBoard : public CParentGameObject
   {
   public:
      CBillBoard();
      virtual ~CBillBoard();

      virtual void setEnvironment();
      virtual void resetEnvironment();

      virtual bool loadTexture(const char * fileName);

      /// This method requires a valid render context
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods - override only some of them
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));

      void setBoundingBox(std::shared_ptr<CShader>& pShader, const glm::vec4& color, const glm::vec3& size = glm::vec3());
      const CBoundingBox* getBoundingBox() const;

      virtual void setVisible(const bool visible);

   private:
      glm::mat4x4 mScaledTRMatrix;
      std::shared_ptr<CBoundingBox> mBoundingBox;
   };
}

#endif // AERO_SIMULATOR_CBILLBOARD_H
