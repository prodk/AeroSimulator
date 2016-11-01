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
{
}

CMovementComponent::~CMovementComponent()
{
}

void CMovementComponent::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      switch (pEvent->getId())
      {
      case eGeneralEvents::MOVE:
         CGameObject& owner = *getOwner();
         owner.move();
         break;
      }
   }
}
