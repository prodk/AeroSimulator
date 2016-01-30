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
   const bool status = GEventManager.registerEvent(eGeneralEvents::UPDATE);
   if (status)
   {
      LOGGL("CMovementComponent: UPDATE event registered: ", status);
   }

   GEventManager.attachEvent(eGeneralEvents::UPDATE, *this);
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
      case eGeneralEvents::UPDATE:
      {
         //LOG("CMovementComponent: UPDATE");
         CGameObject& owner = *getOwner();
         const float deltaTime = owner.getFrameDt();
         CTransformComponent* pTransformComp = componentCast<CTransformComponent>(owner);
         if (pTransformComp)
         {
            CTransform& transform = pTransformComp->getTransform();
            glm::vec3 rotation = transform.getRotate();

            ///@todo: get delta time here somehow
            rotation.x += mRotationSpeed.x * deltaTime;

            transform.setRotate(rotation);
            transform.updateModelMatrix();

            CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(owner);
            if (pRenderableComp)
            {
               ///@todo: change to model matrix here
               pRenderableComp->getRenderable().setMatrix4Param(eShaderMatrix4Params::MVP_MATRIX, transform.getModelMatrix());
            }
         }
      }
      break;
      } // end switch
   }
}
