#include "CCamera.h"
#include "CCommonMath.h"

#include "glm/vec3.hpp"
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

