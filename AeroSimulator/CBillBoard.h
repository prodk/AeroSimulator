#ifndef AERO_SIMULATOR_CBILLBOARD_H
#define AERO_SIMULATOR_CBILLBOARD_H

#include "CGameObject.h"

namespace AeroSimulatorEngine
{
   class CBillBoard : public CGameObject
   {
   public:
      CBillBoard();
      virtual ~CBillBoard();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader); /// Requires a valid render context

      virtual void setEnvironment();
      virtual void resetEnvironment();

      bool loadTexture(const char * fileName);
   };
}

#endif // AERO_SIMULATOR_CBILLBOARD_H
