#include "CTransform.h"

using namespace AeroSimulatorEngine;

CTransform::CTransform()
   : mScale(1.0f, 1.0f, 1.0f)
   , mRotate(0.0f, 0.0f, 0.0f)
   , mTranslate(0.0f, 0.0f, 0.0f)
   , mTRMatrix()
   , mModelMatrix()
{
}

CTransform::~CTransform()
{
}

void CTransform::setScale(const glm::vec3& scale)
{
   mScale = scale;
}

void CTransform::setRotate(const glm::vec3 & rotate)
{
   mRotate = rotate;
}

void CTransform::setTranlate(const glm::vec3 & translate)
{
   mTranslate = translate;
}

///@todo: make a possibility to choose between TR and RT matrices
void CTransform::updateModelMatrix()
{
   updateTranslateRotate();
   mModelMatrix = glm::scale(mTRMatrix, mScale);
}

void CTransform::updateTranslateRotate()
{
   // The matrix will perform these operations in reverse order
   mTRMatrix = glm::translate(glm::mat4x4(), mTranslate);

   // x
   if (mRotate.x > std::numeric_limits<float>::epsilon())
   {
      const float angleX = glm::radians(mRotate.x);
      const glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
      mTRMatrix = glm::rotate(mTRMatrix, angleX, xAxis);
   }

   // y
   if (mRotate.y > std::numeric_limits<float>::epsilon())
   {
      const float angleY = glm::radians(mRotate.y);
      const glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
      mTRMatrix = glm::rotate(mTRMatrix, angleY, yAxis);
   }

   // z
   if (mRotate.z > std::numeric_limits<float>::epsilon())
   {
      const float angleZ = glm::radians(mRotate.z);
      const glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
      mTRMatrix = glm::rotate(mTRMatrix, angleZ, zAxis);
   }
}

void CTransform::updateRotateTranslate()
{
   mTRMatrix = glm::mat4x4();

   // x
   if (mRotate.x > std::numeric_limits<float>::epsilon())
   {
      const float angleX = glm::radians(mRotate.x);
      const glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
      mTRMatrix = glm::rotate(mTRMatrix, angleX, xAxis);
   }

   // y
   if (mRotate.y > std::numeric_limits<float>::epsilon())
   {
      const float angleY = glm::radians(mRotate.y);
      const glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
      mTRMatrix = glm::rotate(mTRMatrix, angleY, yAxis);
   }

   // z
   if (mRotate.z > std::numeric_limits<float>::epsilon())
   {
      const float angleZ = glm::radians(mRotate.z);
      const glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
      mTRMatrix = glm::rotate(mTRMatrix, angleZ, zAxis);
   }

   mTRMatrix = glm::translate(glm::mat4x4(), mTranslate);
}
