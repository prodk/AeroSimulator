#include "CCameraComponent.h"
#include "CEventManager.h"
#include "CGameObject.h"

using namespace AeroSimulatorEngine;

CCameraComponent::CCameraComponent(CGameObject* pOwner)
   : CComponent(pOwner)
   , mTransform()
   , mStateChanges()
   , mStateSigns()
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
         rotate(eChangePitch, deltaTime);
         rotate(eRotateY, deltaTime);
         rotate(eRotateZ, deltaTime);

         if (mStateChanges.any())
         {
            mTransform.updateModelMatrix();
         }
         break;

      case eCameraEvents::INCREASE_PITCH:
         mStateChanges.set(eChangePitch);
         break;

      case eCameraEvents::INCREASE_PITCH_STOP:
         mStateChanges.set(eChangePitch, false);
         break;

      case eCameraEvents::DECREASE_PITCH:
         mStateChanges.set(eChangePitch);
         mStateSigns.set(eChangePitch);
         break;

      case eCameraEvents::DECREASE_PITCH_STOP:
         mStateChanges.set(eChangePitch, false);
         mStateSigns.set(eChangePitch, false);
         break;

      case eCameraEvents::ROTATE_CW:
         mStateChanges.set(eRotateY);
         break;

      case eCameraEvents::ROTATE_CW_STOP:
         mStateChanges.set(eRotateY, false);
         break;

      case eCameraEvents::ROTATE_CCW:
         mStateChanges.set(eRotateY);
         mStateSigns.set(eRotateY);
         break;

      case eCameraEvents::ROTATE_CCW_STOP:
         mStateChanges.set(eRotateY, false);
         mStateSigns.set(eRotateY, false);
         break;

      case eCameraEvents::ROTATE_TO_RIGHT:
         mStateChanges.set(eRotateZ);
         break;

      case eCameraEvents::ROTATE_TO_RIGHT_STOP:
         mStateChanges.set(eRotateZ, false);
         break;

      case eCameraEvents::ROTATE_TO_LEFT:
         mStateChanges.set(eRotateZ);
         mStateSigns.set(eRotateZ);
         break;

      case eCameraEvents::ROTATE_TO_LEFT_STOP:
         mStateChanges.set(eRotateZ, false);
         mStateSigns.set(eRotateZ, false);
         break;
      }
   }
}

void CCameraComponent::rotate(const unsigned int axisId, const float deltaTime)
{
   if ((axisId < 3) && mStateChanges[axisId])
   {
      const float rotateSpeed = 100.0f;  ///@todo: make a member for each axis and adjust this value

      glm::vec3 rotation = mTransform.getRotate();

      if (mStateSigns[axisId])
      {
         rotation[axisId] -= rotateSpeed * deltaTime;
         if (rotation[axisId] < 0.f) rotation[axisId] += 360.f;
      }
      else
      {
         rotation[axisId] += rotateSpeed * deltaTime;
         if (rotation[axisId] > 360.f) rotation[axisId] -= 360.f;
      }

      mTransform.setRotate(rotation);
   }
}

