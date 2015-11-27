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

