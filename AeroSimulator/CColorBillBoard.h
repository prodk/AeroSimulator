#ifndef AERO_SIMULATOR_CCOLOR_BILLBOARD_H
#define AERO_SIMULATOR_CCOLOR_BILLBOARD_H

#include "CCompositeGameObject.h"

namespace AeroSimulatorEngine
{
   class CColorBillBoard : public CCompositeGameObject
   {
   public:
      CColorBillBoard();
      virtual ~CColorBillBoard();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader); /// Requires a valid render context

      /// Composite-related methods
      virtual void add(CCompositeGameObject* child);
      virtual void traverse(std::vector<CCompositeGameObject*>& tree);
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);
   };
}
#endif // AERO_SIMULATOR_CCOLOR_BILLBOARD_H
