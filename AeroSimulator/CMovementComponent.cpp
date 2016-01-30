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
   , mRotationSpeed(1.0f, 0.0f, 0.0f)
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
         CTransformComponent* pTransformComp = componentCast<CTransformComponent>(*getOwner());
         if (pTransformComp)
         {
            CTransform& transform = pTransformComp->getTransform();
            glm::vec3 rotation = transform.getRotate();

            ///@todo: get delta time here somehow
            rotation.x += mRotationSpeed.x;

            transform.setRotate(rotation);
            transform.updateModelMatrix();

            CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(*getOwner());
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
