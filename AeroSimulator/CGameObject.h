#ifndef AERO_SIMULATOR_CGAME_OBJECT_H
#define AERO_SIMULATOR_CGAME_OBJECT_H

#include "CRenderable.h"
#include "glm/vec3.hpp"

namespace AeroSimulatorEngine
{
   class CGameObject : public CRenderable
   {
   public:
      CGameObject();
      virtual ~CGameObject();

      CGameObject(const glm::mat4& parentModelMatrix,
                  const glm::vec3& scale,
                  const glm::vec3& rotate,
                  const glm::vec3& translate);

      virtual void setupGeometry() = 0;
      virtual void setupVBO() = 0;

   protected:
      ///@todo: probably move this to a separate CTransform class/component
      glm::vec3 mScale;   // Scale factors along the parent object axes
      glm::vec3 mRotate;  // Rotation angles in degrees around the parent axes
      glm::vec3 mTranslate;   // Translate along the parent axes

      glm::mat4 mModelMatrix;  // Resulting model matrix for the object

   };

} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CGAME_OBJECT_H
