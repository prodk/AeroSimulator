#ifndef AERO_SIMULATOR_CBILLBOARD_H
#define AERO_SIMULATOR_CBILLBOARD_H

#include "CParentGameObject.h"

namespace AeroSimulatorEngine
{
   class CBoundingBox;

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
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);

      void setBoundingBox(std::shared_ptr<CShader>& pShader);

   private:
      glm::mat4x4 mScaledTRMatrix;
      std::shared_ptr<CBoundingBox> mBoundingBox;
   };
}

#endif // AERO_SIMULATOR_CBILLBOARD_H
