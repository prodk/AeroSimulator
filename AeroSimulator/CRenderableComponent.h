#ifndef AERO_SIMULATOR_CRENDERABLE_COMPONENT_H
#define AERO_SIMULATOR_CRENDERABLE_COMPONENT_H

#include "CComponent.h"
#include <memory>

namespace AeroSimulatorEngine ///@todo: fix this namespace mess when the old CGameObject is removed
{
   class CGameObject;
   class CRenderable;

   ///@todo: probably make it also a CEventHandler
   class CRenderableComponent : public CComponent
   {
   public:
      static unsigned int getId() { return mId; }

      explicit CRenderableComponent(CGameObject* pOwner);
      virtual ~CRenderableComponent();

      virtual void init() override;

      CRenderable & getRenderable() const { return *mRenderable; }

   private:
      static const unsigned int mId = CComponent::RENDERABLE;

      std::unique_ptr<CRenderable> mRenderable;
   };
}

#endif // AERO_SIMULATOR_CRENDERABLE_COMPONENT_H
