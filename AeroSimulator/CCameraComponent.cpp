#include "CCameraComponent.h"
#include "CEventManager.h"

using namespace AeroSimulatorEngine;

CCameraComponent::CCameraComponent(CGameObject* pOwner)
   : CComponent(pOwner)
   , mTransform()
{
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::init()
{
}

void CCameraComponent::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      switch (pEvent->getId())
      {
      case CHANGE_PITCH:
         ///@todo: call changePitch() method here
         break;
      }
   }
}
