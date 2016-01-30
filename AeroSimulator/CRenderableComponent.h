#ifndef AERO_SIMULATOR_CRENDERABLE_COMPONENT_H
#define AERO_SIMULATOR_CRENDERABLE_COMPONENT_H

#include "CComponent.h"
#include "CEventHandler.h"
#include "CRenderable.h"

#include "../AeroSimulator/include/glew.h"
#include <memory>

namespace AeroSimulatorEngine
{
   class CGameObject;
   class CShader;
   class CAppEvent;

   class CRenderableComponent : public CComponent, public CEventHandler
   {
   public:
      static unsigned int getId() { return mId; }

      explicit CRenderableComponent(CGameObject* pOwner);
      virtual ~CRenderableComponent();

      virtual void init() override; ///@todo: probably remove this useless method

      // CEventHandler part
      virtual void handleEvent(CAppEvent *pEvent) override;

      CRenderable & getRenderable();

   private:
      static const unsigned int mId = CComponent::RENDERABLE;

      CRenderable mRenderable;
   };
}

#endif // AERO_SIMULATOR_CRENDERABLE_COMPONENT_H
