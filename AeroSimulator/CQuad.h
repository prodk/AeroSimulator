#ifndef AERO_SIMULATOR_CQUAD_H
#define AERO_SIMULATOR_CQUAD_H

#include "CGameObject.h"

namespace AeroSimulatorEngine
{
   class CQuad : public CGameObject
   {
   public:
      CQuad();
      virtual ~CQuad();
      ///@todo: remove this method
      virtual bool loadTexture(const char* filePath);

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CQUAD_H
