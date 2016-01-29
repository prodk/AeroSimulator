#ifndef AERO_SIMULATOR_CRENDERABLE_COMPONENT_H
#define AERO_SIMULATOR_CRENDERABLE_COMPONENT_H

#include "CComponent.h"
#include <memory>
#include "../AeroSimulator/include/glew.h"
#include "CRenderable.h"

namespace AeroSimulatorEngine
{
   class CGameObject;
   class CShader;

   ///@todo: probably make it also a CEventHandler
   class CRenderableComponent : public CComponent
   {
   public:
      static unsigned int getId() { return mId; }

      explicit CRenderableComponent(CGameObject* pOwner);
      virtual ~CRenderableComponent();

      virtual void init() override;

      CRenderable & getRenderable();

   private:
      static const unsigned int mId = CComponent::RENDERABLE;

      CRenderable mRenderable;
   };
}

#endif // AERO_SIMULATOR_CRENDERABLE_COMPONENT_H
