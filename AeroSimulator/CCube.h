
#ifndef AERO_SIMULATOR_CCUBE_H
#define AERO_SIMULATOR_CCUBE_H

#include "CGameObject.h"

#include <memory>

namespace AeroSimulatorEngine
{
   class CSimpleShader;

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
      virtual void setupGeometry();
      virtual void setupVBO();

      /// Composite-related methods
      virtual void add(CGameObject* child);
      virtual void traverse(std::vector<CGameObject*>& tree);

   public:
      static const int mNumOfElementsPerVertex;
      static const int mStride;
   };

}

#endif // AERO_SIMULATOR_CCUBE_H
