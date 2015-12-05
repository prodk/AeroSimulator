#include "CCamera.h"
#include "CCommonMath.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CCamera::CCamera()
   : CLeafCompositeGameObject()
   , mViewMatrix()
   , mProjectionMatrix()
   , mNonScaledViewMatrix()
{
}

CCamera::~CCamera()
{
}

void CCamera::update()
{
   calculateModelMatrix();

   ///@todo: reconsider this in the attached mode
   /*glm::mat4x4 m;
   m[3] = glm::vec4(7.0f, 5.0f, 0.0f, 1.0);*/
   mViewMatrix = mModelMatrix;
   //mViewMatrix = glm::inverse(mModelMatrix);
}


void CCamera::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   CLeafCompositeGameObject::updateModelMatrix(rootModelMatrix);
   mViewMatrix = mModelMatrix;
   //mViewMatrix = glm::inverse(mModelMatrix);
}
glm::mat3x3 CCamera::getRotationMatrix() const
{
   glm::mat3x3 noTranslate;
   CCommonMath::copyColumn(0, noTranslate, mViewMatrix);
   CCommonMath::copyColumn(1, noTranslate, mViewMatrix);
   CCommonMath::copyColumn(2, noTranslate, mViewMatrix);

   return noTranslate;
}
glm::vec3 CCamera::getRightVector() const
{
   glm::vec3 result;

   /// mat4x4 is composed out of 4 vectors in column-major order
   result.x = mViewMatrix[0].x;
   result.y = mViewMatrix[1].x;
   result.z = mViewMatrix[2].x;

   return result;
}

glm::vec3 CCamera::getUpVector() const
{
   glm::vec3 result;
   result.x = mViewMatrix[0].y;
   result.y = mViewMatrix[1].y;
   result.z = mViewMatrix[2].y;

   return result;
}

glm::vec3 CCamera::getPositionWorldSpace() const
{
   glm::vec3 position;
   // Inversing the view matrix will give camera position in the world space
   // Translate is inverse translation of the view
   position.x = -mViewMatrix[3].x;
   position.y = -mViewMatrix[3].y;
   position.z = -mViewMatrix[3].z;

   glm::mat3x3 noTranslate;
   CCommonMath::copyColumn(0, noTranslate, mViewMatrix);
   CCommonMath::copyColumn(1, noTranslate, mViewMatrix);
   CCommonMath::copyColumn(2, noTranslate, mViewMatrix);

   ///@todo: as we do not have scale, just transpose it
   noTranslate = glm::inverse(noTranslate);

   position = noTranslate * position;

   return position;
}

