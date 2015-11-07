
#ifndef AERO_SIMULATOR_CCUBE_H
#define AERO_SIMULATOR_CCUBE_H

#include "CGameObject.h"

#include <memory>

namespace AeroSimulatorEngine
{
   class CSimpleShader;

   class CCube : public CGameObject
   {
   public:
      CCube();
      virtual ~CCube();

      CCube(const glm::mat4& parentModelMatrix,
            const glm::vec3& scale,
            const glm::vec3& rotate,
            const glm::vec3& translate);

      /// These methods require a valid render context
      virtual void setupGeometry();
      virtual void setupVBO();

   public:
      static const int mNumOfElementsPerVertex;
      static const int mStride;
   };

}

#endif // AERO_SIMULATOR_CCUBE_H
