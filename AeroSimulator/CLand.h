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

      /// These methods require a valid render context
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      bool loadTexture(const char * fileName);
   };
} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLAND_H
