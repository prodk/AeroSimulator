
#ifndef AERO_SIMULATOR_CCUBE_H
#define AERO_SIMULATOR_CCUBE_H

#include "CParentGameObject.h"

namespace AeroSimulatorEngine
{
   ///@todo: probably in the future introduce CLeafGameObject,
   ///but at the moment CCube is the only possible leaf
   class CCube : public CParentGameObject
   {
   public:
      CCube();
      virtual ~CCube();

      CCube(const glm::vec3& scale,
            const glm::vec3& rotate,
            const glm::vec3& translate);

      /// These methods require a valid render context
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods
      virtual void add(CCompositeGameObject* child);
      //virtual void traverse(std::vector<CCompositeGameObject*>& tree);
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      //virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);

   public:
      ///@todo: rename to state that it is for untextured colored cube
      static const int mNumOfElementsPerVertex;
      static const int mStride;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCUBE_H
