#include "CCameraComponent.h"
#include "CEventManager.h"
#include "CGameObject.h"

using namespace AeroSimulatorEngine;

CCameraComponent::CCameraComponent(CGameObject* pOwner)
   : CComponent(pOwner)
   , mTransform()
   , mStateChanges(0u)
{
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      const float pitchSpeed = 200.0f;  ///@todo: make a member and adjust this value
      const float deltaTime = (*getOwner()).getFrameDt();

      switch (pEvent->getId())
      {
      case eCameraEvents::UPDATE_CAMERA:
         ///@todo: place to a member updatePitch()
         if (mStateChanges & eStateChanges::eIncreasePitch)
         {
            glm::vec3 rotation = mTransform.getRotate();
            rotation.x += pitchSpeed * deltaTime;
            mTransform.setRotate(rotation);
         }
         if (mStateChanges & eStateChanges::eDecreasePitch)
         {
            glm::vec3 rotation = mTransform.getRotate();
            rotation.x -= pitchSpeed * deltaTime;
            if (rotation.x < 0.f) rotation.x += 360.f;
            mTransform.setRotate(rotation);
         }

         mTransform.updateModelMatrix();
         break;

      case INCREASE_PITCH: ///@todo: rename to change pitch
         mStateChanges |= eStateChanges::eIncreasePitch;
         break;

      case INCREASE_PITCH_STOP:
         mStateChanges &= ~eStateChanges::eIncreasePitch;
         break;

      case DECREASE_PITCH: ///@todo: rename to change pitch
         mStateChanges |= eStateChanges::eDecreasePitch;
         break;

      case DECREASE_PITCH_STOP:
         mStateChanges &= ~eStateChanges::eDecreasePitch;
         break;
      }
   }
}

