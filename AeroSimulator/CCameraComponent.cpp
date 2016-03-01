#include "CCameraComponent.h"
#include "CEventManager.h"
#include "CGameObject.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CCameraComponent::CCameraComponent(CGameObject* pOwner)
   : CTransformComponent(pOwner)
   //, mTransform()
   , mStateChanges()
   , mStateSigns()
   , mProjectionMatrix()
   //, mParentMatrix()
{
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      switch (pEvent->getId())
      {
      case eCameraEvents::UPDATE_CAMERA:
         update();
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

      case eCameraEvents::ZOOM_OUT:
         mStateChanges.set(eZoom);
         mStateSigns.set(eZoom);
         break;

      case eCameraEvents::ZOOM_OUT_STOP:
         mStateChanges.set(eZoom, false);
         mStateSigns.set(eZoom, false);
         break;

      case eCameraEvents::MOVE_LEFT:
         mStateChanges.set(eMoveLeftRight);
         break;

      case eCameraEvents::MOVE_LEFT_STOP:
         mStateChanges.set(eMoveLeftRight, false);
         break;

      case eCameraEvents::MOVE_RIGHT:
         mStateChanges.set(eMoveLeftRight);
         mStateSigns.set(eMoveLeftRight);
         break;

      case eCameraEvents::MOVE_RIGHT_STOP:
         mStateChanges.set(eMoveLeftRight, false);
         mStateSigns.set(eMoveLeftRight, false);
         break;

      case eCameraEvents::MOVE_UP:
         mStateChanges.set(eMoveUpDown);
         mStateSigns.set(eMoveUpDown);
         break;

      case eCameraEvents::MOVE_UP_STOP:
         mStateChanges.set(eMoveUpDown, false);
         mStateSigns.set(eMoveUpDown, false);
         break;

      case eCameraEvents::MOVE_DOWN:
         mStateChanges.set(eMoveUpDown);
         break;

      case eCameraEvents::MOVE_DOWN_STOP:
         mStateChanges.set(eMoveUpDown, false);
         break;
      }
   }
}

glm::mat4 CCameraComponent::getViewMatrix() const
{
   ///@todo: always return mViewMatrix when caching is implemented
   return mTransform.getInverseRotateTranslate();
}

void CCameraComponent::setProjectionMatrix(const SFrustum & frustum)
{
   mProjectionMatrix = glm::perspective(frustum.mFov, frustum.mAspect, frustum.mNear, frustum.mFar);
}

void CCameraComponent::update()
{
   const float deltaTime = (*getOwner()).getFrameDt();

   rotate(eChangePitch, deltaTime);
   rotate(eRotateY, deltaTime);
   rotate(eRotateZ, deltaTime);

   zoom(deltaTime);
   moveLeftRight(deltaTime);
   moveUpDown(deltaTime);

   ///@todo: think how to not to update tr matrix all the time even when attached to a parent
   if (mStateChanges.any())
   {
      ///@todo: mark cache as dirty and update the mViewMatrix
      mTransform.updateTrMatrix();
      mTransform.setTranslateRotateMatrix(mParentMatrix * mTransform.getTranslateRotateMatrix());
   }

   ///@todo: debug, test parent matrix concept
   //static glm::mat4 parentMatrix;
   //glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
   //parentMatrix = glm::rotate(parentMatrix, 0.1f*deltaTime, yAxis);
   //parentMatrix = glm::translate(parentMatrix, glm::vec3(0.1*deltaTime, 0.0f, 0.0f));
}

void CCameraComponent::rotate(const unsigned int axisId, const float deltaTime)
{
   if ((axisId < 3) && mStateChanges[axisId])
   {
      const float rotateSpeed = 1.5f;  ///@todo: make an array of values for each axis and adjust these values
      const float angle = mStateSigns[axisId] ? -rotateSpeed * deltaTime : rotateSpeed * deltaTime;

      glm::vec3 axis;
      axis[axisId] = 1.0f;

      const glm::mat4 rotationInView = glm::rotate(glm::mat4(), angle, axis);
      const glm::mat4 rotationInWorld = glm::mat4(glm::inverse(glm::mat3(getViewMatrix()))) * rotationInView;
      mTransform.setRotate(rotationInWorld);
   }
}

void CCameraComponent::zoom(const float deltaTime)
{
   moveInViewSpace((bool)mStateChanges[eZoom], mStateSigns[eZoom], glm::vec3(0.0f, 0.0f, 1.0f), deltaTime);
}

void CCameraComponent::moveLeftRight(const float deltaTime)
{
   moveInViewSpace((bool)mStateChanges[eMoveLeftRight], mStateSigns[eMoveLeftRight], glm::vec3(1.0f, 0.0f, 0.0f), deltaTime);
}

void CCameraComponent::moveUpDown(const float deltaTime)
{
   moveInViewSpace((bool)mStateChanges[eMoveUpDown], mStateSigns[eMoveUpDown], glm::vec3(0.0f, 1.0f, 0.0f), deltaTime);
}

void CCameraComponent::moveInViewSpace(const bool state, const bool sign, const glm::vec3 & dir, const float deltaTime)
{
   if (state)
   {
      // Get the direction of the camera
      const glm::mat4 view = getViewMatrix();

      const float zoomStep = 0.1f; ///@todo: adjust and put it in the styles

      glm::vec3 currentPos = mTransform.getTranslate();

      // Translate the camera in the view space, this will provide the correct TRS sequence of matrix operations
      const glm::mat3 viewScaleRotate = glm::mat3(view);
      glm::vec3 posViewSpace = viewScaleRotate * currentPos;
      posViewSpace += sign ? dir * zoomStep : -dir * zoomStep;

      // Transform the new position back to the world space
      currentPos = glm::inverse(viewScaleRotate) * posViewSpace;
      mTransform.setTranslate(currentPos);
   }
}

