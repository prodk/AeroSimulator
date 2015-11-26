#ifndef AERO_SIMULATOR_CBILLBOARD_H
#define AERO_SIMULATOR_CBILLBOARD_H

#include "CCompositeGameObject.h"

namespace AeroSimulatorEngine
{
   ///@todo: create a CLeafNode and inherit CBillBoard and CLine from it
   class CBillBoard : public CCompositeGameObject
   {
   public:
      CBillBoard();
      virtual ~CBillBoard();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader); /// Requires a valid render context

      virtual void setEnvironment();
      virtual void resetEnvironment();

      bool loadTexture(const char * fileName);

      /// Composite-related methods
      virtual void add(CCompositeGameObject* child);
      virtual void traverse(std::vector<CCompositeGameObject*>& tree);
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);
   };
}

#endif // AERO_SIMULATOR_CBILLBOARD_H
