#ifndef AERO_SIMULATOR_CCOMPOSITE_GAME_OBJECT_H
#define AERO_SIMULATOR_CCOMPOSITE_GAME_OBJECT_H

#include "CGameObject.h"

namespace AeroSimulatorEngine
{
   /// This class is used to build hierarchical structures of objects
   class CCompositeGameObject : public CGameObject
   {
   public:
      CCompositeGameObject();
      CCompositeGameObject(const glm::vec3& scale, const glm::vec3& rotate, const glm::vec3& translate);
      virtual ~CCompositeGameObject();

      /// Composite-related methods to be overridden by children
      virtual void add(CCompositeGameObject* child) = 0;

      /// Saves elements to the provided array
      virtual void traverse(std::vector<CCompositeGameObject*>& tree) = 0;

      /// Rendering related composite methods
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix) = 0;
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix) = 0;
      virtual void updateModelMatrix(const glm::mat4x4 & modelMatrix) = 0;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCOMPOSITE_GAME_OBJECT_H
