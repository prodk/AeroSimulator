
#ifndef AERO_SIMULATOR_CCUBE_H
#define AERO_SIMULATOR_CCUBE_H

#include "CGameObject.h"

//#include <memory>

namespace AeroSimulatorEngine
{
   //class CSimpleShader;
   //class CShader;

   ///@todo: probably in the future introduce CLeafGameObject,
   ///but at the moment CCube is the only possible leaf
   class CCube : public CGameObject
   {
   public:
      CCube();
      virtual ~CCube();

      CCube(const CGameObject* parent,
            const glm::vec3& scale,
            const glm::vec3& rotate,
            const glm::vec3& translate);

      /// These methods require a valid render context
      //virtual void setupGeometry(std::shared_ptr<CGeometry>& pGeometry);
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods
      virtual void add(CGameObject* child);
      virtual void traverse(std::vector<CGameObject*>& tree);

      //virtual void updateMatrix(const glm::mat4 & parentMatrix, const glm::mat4& dynamicMatrix);
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);

   public:
      ///@todo: rename to state that it is for untextured colored cube
      static const int mNumOfElementsPerVertex;
      static const int mStride;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCUBE_H
