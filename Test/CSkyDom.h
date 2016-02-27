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
      ///@todo: make addComponent virtual such that no need to declare them in every class
      void addRenderableComponent(std::shared_ptr<CShader>& pShader, const std::string& textureFilePath, const SSphereParams& generateParams);
      void addTransformComponent();
      void addMovementComponent();

      CRenderable & CSkyDom::getRenderable();
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CSKYDOM_H
