#ifndef AERO_SIMULATOR_CLAND_H
#define AERO_SIMULATOR_CLAND_H

#include "CGameObject.h"

///@todo: remove these headers, they are defined in CRenderable.h
//#include "../AeroSimulator/include/glew.h"
//#include <gl/GL.h>

namespace AeroSimulatorEngine
{
   class CLand : public CGameObject
   {
   public:
      CLand();
      virtual ~CLand();

      /// These methods require a valid render context
      //virtual void setupGeometry(std::shared_ptr<CGeometry>& pGeometry);
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods
      virtual void add(CGameObject* child);
      virtual void traverse(std::vector<CGameObject*>& tree);

      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);

      bool loadTexture(const char * fileName);
   };

} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLAND_H
