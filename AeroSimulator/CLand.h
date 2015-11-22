#ifndef AERO_SIMULATOR_CLAND_H
#define AERO_SIMULATOR_CLAND_H

#include "CGameObject.h"

namespace AeroSimulatorEngine
{
   class CLand : public CGameObject
   {
   public:
      CLand();
      virtual ~CLand();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader); /// Requires a valid render context

      bool loadTexture(const char * fileName);
   };
} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLAND_H