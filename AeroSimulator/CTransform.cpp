#include "CTransform.h"

using namespace AeroSimulatorEngine;

CTransform::CTransform()
   : mScale(1.0f, 1.0f, 1.0f)
   , mRotate(0.0f, 0.0f, 0.0f)
   , mTranslate(0.0f, 0.0f, 0.0f)
   , mTRMatrix()
   , mModelMatrix()
   , mTrType(ROTATE_FIRST)
{
}

///@todo: probably remove this constructor
CTransform::CTransform(const CTransform & t)
   : mScale(t.mScale)
   , mRotate(t.mRotate)
   , mTranslate(t.mTranslate)
   , mTRMatrix(t.mTRMatrix)
   , mModelMatrix(t.mModelMatrix)
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
   mRotate = rotate;
}

void CTransform::setTranlate(const glm::vec3 & translate)
{
   mTranslate = translate;
}

///@todo: make a possibility to choose between TR and RT matrices
void CTransform::updateModelMatrix()
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

glm::mat4x4 AeroSimulatorEngine::CTransform::getInverseRotateTranslate() const
{
   // Get the rotation part of the TR matrix
   glm::mat3x3 noTranslate = glm::mat3(mTRMatrix);

   // Its transpose is its inverse
   noTranslate = glm::transpose(noTranslate);

   // Important: do not invert all the 4 translation components- the 4th must stay 1
   glm::mat4x4 result(noTranslate);
   result[3].x = -mTRMatrix[3].x;
   result[3].y = -mTRMatrix[3].y;
   result[3].z = -mTRMatrix[3].z;

   return result;
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

CTransform& CTransform::operator=(const CTransform& transform)
{
   ///@todo: avoid self assignment
   //if (*this != transform)
   {
      mScale = transform.mScale;
      mRotate = transform.mRotate;
      mTranslate = transform.mTranslate;
      mTRMatrix = transform.mTRMatrix;
      mModelMatrix = transform.mModelMatrix;
      mTrType = transform.mTrType;
   }

   return *this;
}
