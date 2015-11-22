#include "CCamera.h"
#include "CCommonMath.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CCamera::CCamera()
   : mRotate()
   , mTranslate()
   , mViewMatrix()
   , mProjectionMatrix()
{
}

CCamera::~CCamera()
{
}

void CCamera::translate(const glm::vec3 & distance)
{
   mTranslate = glm::mat4(1.0f);
   mTranslate = glm::translate(mTranslate, distance);
   mViewMatrix = mTranslate * mRotate;
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

   mViewMatrix = mTranslate * mRotate;
}

void CCamera::resetView()
{
   mViewMatrix = glm::mat4(1.0f);
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

