
#ifndef AERO_SIMULATOR_CCUBE_H
#define AERO_SIMULATOR_CCUBE_H

//#include "CParentGameObject.h"
#include "CGameObject.h"
#include <algorithm>

namespace AeroSimulatorEngine
{
   ///@todo: introduce a special healthbar cube and move mHealthBar there.
   ///@todo: this is because the sky box and bounding box do not need health bars.
   //class CCube : public CParentGameObject
   class CCube : public CGameObject
   {
   public:
      CCube();
      virtual ~CCube();

      /*CCube(const glm::vec3& scale,
            const glm::vec3& rotate,
            const glm::vec3& translate);*/

      /// This method requires a valid render context
      /*virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods - override only some of them
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));*/

      //void setupHealthBar(std::shared_ptr<CShader>& pShader);
      //virtual bool loadTexture(const char * fileName);
      //void translateHealthBar(const glm::vec3& shift);
      void setHealth(float health);

   public:
      ///@todo: rename to state that it is for untextured colored cube
      static const int mNumOfElementsPerVertex;
      static const int mStride;

   private:
      //glm::mat4x4 mScaledTRMatrix;
      //std::shared_ptr<CCompositeGameObject> mHealthBar;
      //glm::vec3 mHealthBarShift;
      float mHealthBarWidth;
      float mHealthbarHeight;
      //float mHealthValue;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCUBE_H
