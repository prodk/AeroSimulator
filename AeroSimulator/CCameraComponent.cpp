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
      const float deltaTime = (*getOwner()).getFrameDt();

      switch (pEvent->getId())
      {
      case eCameraEvents::UPDATE_CAMERA:
         updatePitch(deltaTime);
         rotateAroundAxisOfSymmetry(deltaTime);
         mTransform.updateModelMatrix();
         break;

      case eCameraEvents::INCREASE_PITCH:
         mStateChanges |= eStateChanges::eIncreasePitch;
         break;

      case eCameraEvents::INCREASE_PITCH_STOP:
         mStateChanges &= ~eStateChanges::eIncreasePitch;
         break;

      case eCameraEvents::DECREASE_PITCH:
         mStateChanges |= eStateChanges::eDecreasePitch;
         break;

      case eCameraEvents::DECREASE_PITCH_STOP:
         mStateChanges &= ~eStateChanges::eDecreasePitch;
         break;

      case eCameraEvents::ROTATE_CW:
         mStateChanges |= eStateChanges::eRotateCw;
         break;

      case eCameraEvents::ROTATE_CW_STOP:
         mStateChanges &= ~eStateChanges::eRotateCw;
         break;
      }
   }
}

void CCameraComponent::updatePitch(const float deltaTime)
{
   const float pitchSpeed = 200.0f;  ///@todo: make a member and adjust this value

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
}

void CCameraComponent::rotateAroundAxisOfSymmetry(const float deltaTime)
{
   const float rotateSpeed = 200.0f;  ///@todo: make a member and adjust this value

   if (mStateChanges & eStateChanges::eRotateCw)
   {
      glm::vec3 rotation = mTransform.getRotate();
      rotation.y += rotateSpeed * deltaTime;
      mTransform.setRotate(rotation);
   }
}

