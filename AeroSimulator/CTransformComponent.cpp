#include "CTransformComponent.h"
#include "CGameObject.h"
#include "CEventManager.h"
#include "CRenderableComponent.h"

using namespace AeroSimulatorEngine;

CTransformComponent::CTransformComponent(CGameObject* pOwner)
   : CComponent(pOwner)
   , mTransform()
{
}

CTransformComponent::~CTransformComponent()
{
}

void CTransformComponent::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      switch (pEvent->getId())
      {
      case eGeneralEvents::UPDATE_TRANSFORM:
         update();
         break;
      }
   }
}

void CTransformComponent::update()
{
   CGameObject& owner = *getOwner();
   const float deltaTime = owner.getFrameDt();

   CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(owner);
   if (pRenderableComp)
   {
      pRenderableComp->getRenderable().setMatrix4Param(eShaderMatrix4Params::MODEL_MATRIX, mTransform.getModelMatrix());
   }
}
