#include "CCamera.h"
#include "CCommonMath.h"
#include "CCameraComponent.h"
#include "CTransform.h"
#include "CLog.h"
#include "CEventHandler.h"
#include "CEventManager.h"

#include "glm/gtc/matrix_transform.hpp"
#include <cassert>

using namespace AeroSimulatorEngine;

CCamera::CCamera(const CTransform& transform)
   : CGameObject()
   , mId(-1)
   //, mViewMatrix()
   //, mProjectionMatrix()
   //, mNonScaledViewMatrix()
   //, mLookAtMatrix()
{
   if (addComponent<CCameraComponent>())
   {
      LOG("* CCamera setting up the camera component");
      getTransform() = transform;
      //getTransform().setTranslationFirst(true); // Camera is first translated from the look at point and then rotated around it
      getTransform().updateModelMatrix();

      registerEvents();
   }
}

CCamera::~CCamera()
{
}

glm::mat4 CCamera::getViewMatrix()
{
   return getTransform().getInverseRotateTranslate();
}

CTransform & CCamera::getTransform()
{
   CCameraComponent* cameraComp = componentCast<CCameraComponent>(*this);
   assert(cameraComp);

   return cameraComp->getTransform();
}

void CCamera::registerEvents()
{
   if (GEventManager.registerEvent(eCameraEvents::UPDATE_CAMERA))
   {
      LOG("CCamera: UPDATE_CAMERA event registered");
   }

   if (GEventManager.registerEvent(eCameraEvents::INCREASE_PITCH))
   {
      LOG("CCamera: INCREASE_PITCH event registered");
   }

   if (GEventManager.registerEvent(eCameraEvents::INCREASE_PITCH_STOP))
   {
      LOG("CCamera: INCREASE_PITCH_STOP event registered");
   }

   if (GEventManager.registerEvent(eCameraEvents::DECREASE_PITCH))
   {
      LOG("CCamera: DECREASE_PITCH event registered");
   }

   if (GEventManager.registerEvent(eCameraEvents::DECREASE_PITCH_STOP))
   {
      LOG("CCamera: DECREASE_PITCH_STOP event registered");
   }

   CCameraComponent* cameraComp = componentCast<CCameraComponent>(*this);
   if (cameraComp)
   {
      GEventManager.attachEvent(eCameraEvents::UPDATE_CAMERA, *cameraComp);
      GEventManager.attachEvent(eCameraEvents::INCREASE_PITCH, *cameraComp);
      GEventManager.attachEvent(eCameraEvents::INCREASE_PITCH_STOP, *cameraComp);
      GEventManager.attachEvent(eCameraEvents::DECREASE_PITCH, *cameraComp);
      GEventManager.attachEvent(eCameraEvents::DECREASE_PITCH_STOP, *cameraComp);
   }
}

//void CCamera::update()
//{
   //calculateModelMatrix();

   //mViewMatrix = mModelMatrix * mLookAtMatrix;
//}

//void CCamera::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
//{
//   calculateModelMatrix();
//
//   /*glm::vec3 translate;
//   translate.x = -mParentTRMatrix[3].x;
//   translate.y = -mParentTRMatrix[3].y;
//   translate.z = -mParentTRMatrix[3].z;
//
//   glm::mat4x4 translateParent;
//   translateParent = glm::translate(translateParent, translate);
//
//   mViewMatrix = mModelMatrix * mLookAtMatrix * translateParent;*/
//
//   glm::mat4x4 inverseParent;
//   inverseParent = glm::inverse(mParentTRMatrix);
//   mViewMatrix = mModelMatrix * mLookAtMatrix * inverseParent;
//}

//glm::mat3x3 CCamera::getRotationMatrix() const
//{
//   glm::mat3x3 noTranslate;
//   CCommonMath::copyColumn(0, noTranslate, mViewMatrix);
//   CCommonMath::copyColumn(1, noTranslate, mViewMatrix);
//   CCommonMath::copyColumn(2, noTranslate, mViewMatrix);
//
//   return noTranslate;
//}

//void CCamera::translateLookAt(const glm::vec3& shift)
//{
   ///Transform the shift from camera to world space using inverse transform
   //glm::mat3x3 noTranslate;
   //CCommonMath::copyColumn(0, noTranslate, mModelMatrix);
   //CCommonMath::copyColumn(1, noTranslate, mModelMatrix);
   //CCommonMath::copyColumn(2, noTranslate, mModelMatrix);

   //// For orthonormal matrices inverse==transpose
   //noTranslate = glm::transpose(noTranslate);

   ////// Transform the translation part
   //glm::vec3 translate = noTranslate * shift;

   //mLookAtMatrix = glm::translate(mLookAtMatrix, translate);
//}

//glm::vec3 CCamera::getLookAtPosition() const
//{
//   glm::vec3 result;
//
//   result.x = mLookAtMatrix[3].x;
//   result.y = mLookAtMatrix[3].y;
//   result.z = mLookAtMatrix[3].z;
//
//   return result;
//}
//
//glm::vec3 CCamera::getRightVector() const
//{
//   glm::vec3 result;
//
//   /// mat4x4 is composed out of 4 vectors in column-major order
//   result.x = mViewMatrix[0].x;
//   result.y = mViewMatrix[1].x;
//   result.z = mViewMatrix[2].x;
//
//   return result;
//}
//
//glm::vec3 CCamera::getUpVector() const
//{
//   glm::vec3 result;
//   result.x = mViewMatrix[0].y;
//   result.y = mViewMatrix[1].y;
//   result.z = mViewMatrix[2].y;
//
//   return result;
//}
//
//glm::vec3 CCamera::getPositionWorldSpace() const
//{
//   glm::vec3 position;
//   // Inversing the view matrix will give camera position in the world space
//   // Translate is inverse translation of the view
//   position.x = -mViewMatrix[3].x;
//   position.y = -mViewMatrix[3].y;
//   position.z = -mViewMatrix[3].z;
//
//   glm::mat3x3 noTranslate;
//   CCommonMath::copyColumn(0, noTranslate, mViewMatrix);
//   CCommonMath::copyColumn(1, noTranslate, mViewMatrix);
//   CCommonMath::copyColumn(2, noTranslate, mViewMatrix);
//
//   /// Transpose==inverse for orthonormal matrices (rotation without scaling)
//   noTranslate = glm::transpose(noTranslate);
//
//   position = noTranslate * position;
//
//   return position;
//}
//
//void CCamera::setXzDirection(const glm::vec3 & direction)
//{
//   mViewMatrix[0].z = direction.x;
//   mViewMatrix[2].z = direction.z;
//   //mViewMatrix = glm::mat4x4();
//}

