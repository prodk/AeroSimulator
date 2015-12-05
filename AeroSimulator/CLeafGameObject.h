#ifndef AERO_SIMULATOR_CLEAF_COMPOSITE_GAME_OBJECT_H
#define AERO_SIMULATOR_CLEAF_COMPOSITE_GAME_OBJECT_H

#include "CCompositeGameObject.h"

namespace AeroSimulatorEngine
{
   class CLeafGameObject : public CCompositeGameObject
   {
   public:
      CLeafGameObject();
      virtual ~CLeafGameObject();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader); /// Requires a valid render context

      /// Composite-related methods
      virtual void add(CCompositeGameObject* child);
      virtual void traverse(std::vector<CCompositeGameObject*>& tree);
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));
   };
} // namespace AeroSimulatorEngine

#endif AERO_SIMULATOR_CLEAF_COMPOSITE_GAME_OBJECT_H
