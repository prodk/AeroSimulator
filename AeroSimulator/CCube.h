
#ifndef AERO_SIMULATOR_CCUBE_H
#define AERO_SIMULATOR_CCUBE_H

#include "CParentGameObject.h"
#include <algorithm>

namespace AeroSimulatorEngine
{
   class CCube : public CParentGameObject
   {
   public:
      CCube();
      virtual ~CCube();

      CCube(const glm::vec3& scale,
            const glm::vec3& rotate,
            const glm::vec3& translate);

      /// This method requires a valid render context
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods - override only some of them
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);

      void setupHealthBar(std::shared_ptr<CShader>& pShader);

      virtual bool loadTexture(const char * fileName);

      void translateHealthBar(const glm::vec3& shift);

      void setHealth(float health) { mHealthValue = std::min<float>(1.0, std::max<float>(0.0, health)); }

   public:
      ///@todo: rename to state that it is for untextured colored cube
      static const int mNumOfElementsPerVertex;
      static const int mStride;

   private:
      glm::mat4x4 mScaledTRMatrix;
      std::shared_ptr<CCompositeGameObject> mHealthBar;
      glm::vec3 mHealthBarShift;
      float mHealthBarWidth;
      float mHealthbarHeight;
      float mHealthValue;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCUBE_H
