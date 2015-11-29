#include "CCamera.h"
#include "CCommonMath.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CCamera::CCamera()
   : mScale()
   , mRotate()
   , mTranslate()
   , mViewMatrix()
   , mProjectionMatrix()
   , mNonScaledViewMatrix()
{
}

CCamera::~CCamera()
{
}

void CCamera::scale(const glm::vec3 & scales)
{
   mScale = glm::mat4x4(1.0f);
   mScale = glm::scale(mScale, scales);
}

void CCamera::translate(const glm::vec3 & distance)
{
   mTranslate = glm::mat4(1.0f);
   mTranslate = glm::translate(mTranslate, distance);
}

void CCamera::rotate(const glm::vec3 & angles)
{
   ///@todo: check that angles are non-zero
   mRotate = glm::mat4(1.0f);

   const float angleX = CCommonMath::degToRad(angles.x);
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   mRotate = glm::rotate(mRotate, angleX, xAxis);

   const float angleY = CCommonMath::degToRad(angles.y);
   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   mRotate = glm::rotate(mRotate, angleY, yAxis);

   const float angleZ = CCommonMath::degToRad(angles.z);
   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   mRotate = glm::rotate(mRotate, angleZ, zAxis);
}

void CCamera::resetView()
{
   mViewMatrix = glm::mat4(1.0f);
}

void CCamera::update()
{
   mNonScaledViewMatrix = mTranslate * mRotate;
   mViewMatrix = mNonScaledViewMatrix * mScale;
}

glm::vec3 CCamera::getRightVector() const
{
   glm::vec3 result;

   /// mat4x4 is composed out of 4 vectors in column-major order
   result.x = mNonScaledViewMatrix[0].x;
   result.y = mNonScaledViewMatrix[1].x;
   result.z = mNonScaledViewMatrix[2].x;

   return result;
}

glm::vec3 CCamera::getUpVector() const
{
   glm::vec3 result;
   result.x = mNonScaledViewMatrix[0].y;
   result.y = mNonScaledViewMatrix[1].y;
   result.z = mNonScaledViewMatrix[2].y;

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
   copyColumn(0, noTranslate, mViewMatrix);
   copyColumn(1, noTranslate, mViewMatrix);
   copyColumn(2, noTranslate, mViewMatrix);

   noTranslate = glm::inverse(noTranslate);

   position = noTranslate * position;

   return position;
}

void CCamera::copyColumn(int columnId, glm::mat3x3 & out, const glm::mat4x4 in) const
{
   out[columnId].x = in[columnId].x;
   out[columnId].y = in[columnId].y;
   out[columnId].z = in[columnId].z;
}

