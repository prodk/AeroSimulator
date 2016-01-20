#ifndef AERO_SIMULATOR_CRENDERABLE_COMPONENT_H
#define AERO_SIMULATOR_CRENDERABLE_COMPONENT_H

#include "CComponent.h"
#include <memory>

namespace AeroSimulatorEngine ///@todo: fix this namespace mess when the old CGameObject is removed
{
   class NGame::CGameObject;
   class CRenderable;

   ///@todo: probably make it also a CEventHandler
   class CRenderableComponent : public NGame::CComponent
   {
   public:
      static unsigned int getId() { return mId; }

      explicit CRenderableComponent(NGame::CGameObject* pOwner);
      virtual ~CRenderableComponent();

      virtual void init() override;

      CRenderable & getRenderable() const { return *mRenderable; }

   private:
      static const unsigned int mId = 1u;    ///@todo: probably use enum to avoid magic numbers

      std::unique_ptr<CRenderable> mRenderable;
   };
}

#endif // AERO_SIMULATOR_CRENDERABLE_COMPONENT_H
