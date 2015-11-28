#ifndef AERO_SIMULATOR_CLEAF_COMPOSITE_GAME_OBJECT_H
#define AERO_SIMULATOR_CLEAF_COMPOSITE_GAME_OBJECT_H

#include "CCompositeGameObject.h"

///@todo: rename to CLeafGameObject

namespace AeroSimulatorEngine
{
   class CLeafCompositeGameObject : public CCompositeGameObject
   {
   public:
      CLeafCompositeGameObject();
      virtual ~CLeafCompositeGameObject();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader); /// Requires a valid render context

      /// Composite-related methods
      virtual void add(CCompositeGameObject* child);
      virtual void traverse(std::vector<CCompositeGameObject*>& tree);
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);
   };
} // namespace AeroSimulatorEngine

#endif AERO_SIMULATOR_CLEAF_COMPOSITE_GAME_OBJECT_H
