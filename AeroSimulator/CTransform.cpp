#include "CTransform.h"

using namespace AeroSimulatorEngine;

CTransform::CTransform()
   : mScale(1.0f, 1.0f, 1.0f)
   , mTranslate(0.0f, 0.0f, 0.0f)
   , mRotationMatrix()
   , mTRMatrix()
   , mTrType(ROTATE_FIRST)
   , mState()
{
}

CTransform::CTransform(const CTransform & t)
   : mScale(t.mScale)
   , mTranslate(t.mTranslate)
   , mRotationMatrix(t.mRotationMatrix)
   , mTRMatrix(t.mTRMatrix)
   , mTrType(t.mTrType)
   , mState()
{
}

CTransform::~CTransform()
{
}

void CTransform::setScale(const glm::vec3& scale)
{
   if (scale != mScale)
   {
      mScale = scale;
      mState.set(eTransformState::eParentChanged);
   }
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

   mState.set(eTransformState::eTrChanged);
}

void CTransform::setRotate(const glm::mat4 & rotationMatrix)
{
   if (rotationMatrix != mRotationMatrix)
   {
      mRotationMatrix = rotationMatrix;
      mState.set(eTransformState::eTrChanged);
   }
}

void CTransform::setTranslate(const glm::vec3 & translate)
{
   if (translate != mTranslate)
   {
      mTranslate = translate;
      mState.set(eTransformState::eTrChanged);
   }
}

glm::mat4 CTransform::getModelMatrix() const
{
   ///@todo: check for state here and update mModelMatrix if
   return glm::scale(mTRMatrix, mScale);
}

void CTransform::setTranslateRotateMatrix(const glm::mat4 & m)
{
   if (m != mTRMatrix)
   {
      mTRMatrix = m;
      mState.set(eTransformState::eParentChanged); ///@todo: reconsider this
   }
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
   ///@todo: introduce mViewMatrix and return it (updating if the state requires)
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
