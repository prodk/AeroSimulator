#include "CCamera.h"
#include "CCommonMath.h"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CCamera::CCamera()
   : mViewMatrix()
   , mProjectionMatrix()
{
}

CCamera::~CCamera()
{
}

void CCamera::translate(const glm::vec3 & distance)
{
   mViewMatrix = glm::translate(mViewMatrix, distance);
}

void CCamera::rotate(const glm::vec3 & angles)
{
   const float angleX = CCommonMath::degToRad(angles.x);
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   mViewMatrix = glm::rotate(mViewMatrix, angleX, xAxis);

   const float angleY = CCommonMath::degToRad(angles.y);
   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   mViewMatrix = glm::rotate(mViewMatrix, angleY, yAxis);

   const float angleZ = CCommonMath::degToRad(angles.z);
   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   mViewMatrix = glm::rotate(mViewMatrix, angleZ, zAxis);
}

void CCamera::resetView()
{
   mViewMatrix = glm::mat4(1.0f);
}

