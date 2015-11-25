#ifndef AERO_SIMULATOR_CLINE_H
#define AERO_SIMULATOR_CLINE_H

#include "CCompositeGameObject.h"
#include <memory>

namespace AeroSimulatorEngine
{
   class CLine : public CCompositeGameObject
   {
   public:
      CLine();
      virtual ~CLine();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods
      virtual void add(CCompositeGameObject* child);
      virtual void traverse(std::vector<CCompositeGameObject*>& tree);
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLINE_H
