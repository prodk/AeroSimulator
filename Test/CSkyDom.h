#ifndef AERO_SIMULATOR_CSKYDOM_H
#define AERO_SIMULATOR_CSKYDOM_H

#include "../AeroSimulator/CGameObject.h"
#include "../AeroSimulator/CUtils.h"

#include <string>

namespace AeroSimulatorEngine
{
   class CShader;
   class CRenderable;

   class CSkyDom : public CGameObject
   {
   public:
      CSkyDom(const int id,
              const int type,
              std::shared_ptr<CShader>& pShader,
              const std::string& textureFilePath,
              const SSphereParams& generateParams);

      virtual ~CSkyDom();

   private:
      void addRenderableComponent(std::shared_ptr<CShader>& pShader, const std::string& textureFilePath, const SSphereParams& generateParams);
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CSKYDOM_H
