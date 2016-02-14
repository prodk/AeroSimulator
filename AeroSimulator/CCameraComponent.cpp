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
         zoom(deltaTime);

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

      case eCameraEvents::ROTATE_Y_CW:
         mStateChanges.set(eRotateY);
         break;

      case eCameraEvents::ROTATE_Y_CW_STOP:
         mStateChanges.set(eRotateY, false);
         break;

      case eCameraEvents::ROTATE_Y_CCW:
         mStateChanges.set(eRotateY);
         mStateSigns.set(eRotateY);
         break;

      case eCameraEvents::ROTATE_Y_CCW_STOP:
         mStateChanges.set(eRotateY, false);
         mStateSigns.set(eRotateY, false);
         break;

      case eCameraEvents::ROTATE_Z_CW:
         mStateChanges.set(eRotateZ);
         break;

      case eCameraEvents::ROTATE_Z_CW_STOP:
         mStateChanges.set(eRotateZ, false);
         break;

      case eCameraEvents::ROTATE_Z_CCW:
         mStateChanges.set(eRotateZ);
         mStateSigns.set(eRotateZ);
         break;

      case eCameraEvents::ROTATE_Z_CCW_STOP:
         mStateChanges.set(eRotateZ, false);
         mStateSigns.set(eRotateZ, false);
         break;

      case eCameraEvents::ZOOM_IN:
         mStateChanges.set(eZoom);
         break;

      case eCameraEvents::ZOOM_IN_STOP:
         mStateChanges.set(eZoom, false);
         break;
      }
   }
}

void CCameraComponent::rotate(const unsigned int axisId, const float deltaTime)
{
   if ((axisId < 3) && mStateChanges[axisId])
   {
      const float rotateSpeed = 50.0f;  ///@todo: make an array of values for each axis and adjust these values

      glm::vec3 rotation = mTransform.getRotate();

      ///@todo: probably change the signs: if set use +, if not use -
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

void CCameraComponent::zoom(const float deltaTime)
{
   // Get the direction of the camera
   
   if (mStateSigns[eZoom])
   {
      // Zoom out, increase the distance
   }
   else
   {
      // Zoom in, decrease the distance
   }
}

