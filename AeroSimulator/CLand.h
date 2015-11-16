#ifndef AERO_SIMULATOR_CLAND_H
#define AERO_SIMULATOR_CLAND_H

#include "CGameObject.h"

#include "../AeroSimulator/include/glew.h"
#include <gl/GL.h>

namespace AeroSimulatorEngine
{
   class CLand : public CGameObject
   {
   public:
      CLand();
      virtual ~CLand();

      /// These methods require a valid render context
      virtual void setupGeometry(std::shared_ptr<CGeometry>& pGeometry);
      virtual void setupShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods
      virtual void add(CGameObject* child);
      virtual void traverse(std::vector<CGameObject*>& tree);
   };

} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLAND_H
