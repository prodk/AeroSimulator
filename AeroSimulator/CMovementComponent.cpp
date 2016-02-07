#include "CMovementComponent.h"
#include "CEventManager.h"
#include "CTransformComponent.h"
#include "CGameObject.h"
#include "CRenderableComponent.h"
#include "CRenderable.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

CMovementComponent::CMovementComponent(CGameObject* pOwner)
   : CComponent(pOwner)
   , CEventHandler()
   , mRotationSpeed(10.0f, 0.0f, 0.0f)
{
}

CMovementComponent::~CMovementComponent()
{
}

void CMovementComponent::init()
{
}

void CMovementComponent::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      switch (pEvent->getId())
      {
         ///@todo: rename to update movements
      case eGeneralEvents::UPDATE:
      {
         //LOG("CMovementComponent: UPDATE");
         CGameObject& owner = *getOwner();
         const float deltaTime = owner.getFrameDt();
         CTransformComponent* pTransformComp = componentCast<CTransformComponent>(owner);
         if (pTransformComp)
         {
            ///@todo: add caching to the transform component such that we update the transform only if needed
            CTransform& transform = pTransformComp->getTransform();
            glm::vec3 rotation = transform.getRotate();

            //rotation.x += mRotationSpeed.x * deltaTime;

            transform.setRotate(rotation);
            transform.updateModelMatrix();

            CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(owner);
            if (pRenderableComp)
            {
               ///@todo: change to model matrix here
               pRenderableComp->getRenderable().setMatrix4Param(eShaderMatrix4Params::MODEL_MATRIX, transform.getModelMatrix());
            }
         }
      }
      break;
      } // end switch
   }
}
