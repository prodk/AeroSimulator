#include "CTransformComponent.h"
#include "CGameObject.h"
#include "CEventManager.h"
#include "CRenderableComponent.h"
#include "CCameraComponent.h"

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
      case eGeneralEvents::UPDATE_TRANSFORM: ///@todo: rename this message to set model matrix to renderable
         update();
         break;
      }
   }
}

///@note: this must be called after all movements/rotations etc. as here we set the model matrix
void CTransformComponent::update()
{
   CGameObject& owner = *getOwner();
   const float deltaTime = owner.getFrameDt();

   // Update translate/rotate matrices of all the children that have CCameraComponent
   owner.updateChildrensMatrix<CCameraComponent>();

   ///@todo: add for the CTransformComponent update children here

   CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(owner);
   if (pRenderableComp)
   {
      pRenderableComp->getRenderable().setMatrix4Param(eShaderMatrix4Params::MODEL_MATRIX, mTransform.getModelMatrix());
   }
}
