#include "CTransform.h"

using namespace AeroSimulatorEngine;

CTransform::CTransform()
   : mScale(1.0f, 1.0f, 1.0f)
   , mTranslate(0.0f, 0.0f, 0.0f)
   , mRotationMatrix()
   , mTRMatrix()
   , mTrType(ROTATE_FIRST)
{
}

CTransform::CTransform(const CTransform & t)
   : mScale(t.mScale)
   , mTranslate(t.mTranslate)
   , mRotationMatrix(t.mRotationMatrix)
   , mTRMatrix(t.mTRMatrix)
   , mTrType(t.mTrType)
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
   mRotationMatrix = glm::mat4();

   // x
   if (std::fabs(rotate.x) > std::numeric_limits<float>::epsilon())
   {
      const float angleX = glm::radians(rotate.x);
      const glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
      mRotationMatrix = glm::rotate(mRotationMatrix, angleX, xAxis);
   }

   // y
   if (std::fabs(rotate.y) > std::numeric_limits<float>::epsilon())
   {
      const float angleY = glm::radians(rotate.y);
      const glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
      mRotationMatrix = glm::rotate(mRotationMatrix, angleY, yAxis);
   }

   // z
   if (std::fabs(rotate.z) > std::numeric_limits<float>::epsilon())
   {
      const float angleZ = glm::radians(rotate.z);
      const glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
      mRotationMatrix = glm::rotate(mRotationMatrix, angleZ, zAxis);
   }
}

void CTransform::setRotate(const glm::mat4 & rotationMatrix)
{
   mRotationMatrix = rotationMatrix;
}

void CTransform::setTranslate(const glm::vec3 & translate)
{
   mTranslate = translate;
}

glm::mat4 CTransform::getModelMatrix() const
{
   return glm::scale(mTRMatrix, mScale);
}

void CTransform::setTranslateRotateMatrix(const glm::mat4 & m)
{
   mTRMatrix = m;
}

void CTransform::updateTrMatrix()
{
   switch (mTrType)
   {
   case TRANSLATE_FIRST:
      updateRotateTranslate();
      break;

   case ROTATE_FIRST:
   default:
      updateTranslateRotate();
      break;
   }
}

void CTransform::updateTranslateRotate()
{
   mTRMatrix = glm::translate(glm::mat4x4(), mTranslate);
   mTRMatrix *= mRotationMatrix;
}

void CTransform::updateRotateTranslate()
{
   mTRMatrix = mRotationMatrix * glm::translate(glm::mat4x4(), mTranslate);
}

glm::mat4x4 AeroSimulatorEngine::CTransform::getInverseRotateTranslate() const
{
   ///@todo: perform calculations only whent mTRMatrix has changed, not all the time as it is now!
   glm::mat4 directInverse = glm::inverse(mTRMatrix);
   return directInverse;
}

void CTransform::setTranslationFirst(bool first)
{
   if (first)
   {
      mTrType = TRANSLATE_FIRST;
   }
   else
   {
      mTrType = ROTATE_FIRST;
   }
}

bool CTransform::operator!=(const CTransform& transform)
{
   return (mScale != transform.mScale)
       || (mTranslate != transform.mTranslate)
       || (mRotationMatrix != transform.mRotationMatrix)
       || (mTRMatrix != transform.mTRMatrix)
       || (mTrType != transform.mTrType);
}

CTransform& CTransform::operator=(const CTransform& transform)
{
   if (*this != transform)
   {
      mScale = transform.mScale;
      mTranslate = transform.mTranslate;
      mRotationMatrix = transform.mRotationMatrix;
      mTRMatrix = transform.mTRMatrix;
      mTrType = transform.mTrType;
   }

   return *this;
}
