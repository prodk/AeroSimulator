#include "CCameraComponent.h"
#include "CEventManager.h"
#include "CGameObject.h"

using namespace AeroSimulatorEngine;

CCameraComponent::CCameraComponent(CGameObject* pOwner)
   : CComponent(pOwner)
   , mTransform()
   , mIncreasePitch(false)
   , mDecreasePitch(false)
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
      const float pitchSpeed = 200.0f;  ///@todo: make a member and adjust this value
      const float deltaTime = (*getOwner()).getFrameDt();

      switch (pEvent->getId())
      {
      case eCameraEvents::UPDATE_CAMERA:
         ///@todo: place to a member updatePitch()
         if (mIncreasePitch)
         {
            glm::vec3 rotation = mTransform.getRotate();
            rotation.x += pitchSpeed * deltaTime;
            mTransform.setRotate(rotation);
         }
         if (mDecreasePitch)
         {
            glm::vec3 rotation = mTransform.getRotate();
            rotation.x -= pitchSpeed * deltaTime;
            if (rotation.x < 0.f) rotation.x += 360.f;
            mTransform.setRotate(rotation);
         }

         mTransform.updateModelMatrix();
         break;

      case INCREASE_PITCH: ///@todo: rename to change pitch
         mIncreasePitch = true;
         break;

      case INCREASE_PITCH_STOP:
         mIncreasePitch = false;
         break;

      case DECREASE_PITCH: ///@todo: rename to change pitch
         mDecreasePitch = true;
         break;

      case DECREASE_PITCH_STOP:
         mDecreasePitch = false;
         break;
      }
   }
}

