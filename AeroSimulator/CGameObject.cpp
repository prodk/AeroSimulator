#include "CGameObject.h"
#include "CCommonMath.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CGameObject::CGameObject()
   : CRenderable()
   , mScale()
   , mRotate()
   , mTranslate()
   , mTRMatrix()
   , mIsLeaf(false)
   , mIsDynamic(false)
{
}

CGameObject::~CGameObject()
{
}

CGameObject::CGameObject(const CGameObject* parent,
                         const glm::vec3 & scale,
                         const glm::vec3 & rotate,
                         const glm::vec3 & translate)
   : CRenderable()
   , mScale(scale)
   , mRotate(rotate)
   , mTranslate(translate)
   , mIsLeaf(false)
{
}

void CGameObject::updateMatrix(const glm::mat4 & parentMatrix, const glm::mat4& dynamicMatrix)
{
}

glm::mat4 CGameObject::getChildTRMatrix(std::size_t childId) const
{
   return glm::mat4();
}

void CGameObject::resetModelMatrix(const glm::mat4& matrix)
{
   mModelMatrix = matrix;
}

void CGameObject::resetTRMatrix(const glm::mat4 & matrix)
{
   mTRMatrix = matrix;
   mModelMatrix = mTRMatrix;
}

glm::mat4 CGameObject::getTRMatrix() const
{
   return mTRMatrix;
}

void CGameObject::scale(const glm::vec3& scale)
{
   mScale = scale;
   mModelMatrix = glm::scale(mModelMatrix, scale);
}

void CGameObject::rotate(const glm::vec3& rotate)
{
   mRotate = rotate;

   const float angleX = CCommonMath::degToRad(rotate.x);
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   mTRMatrix = glm::rotate(mTRMatrix, angleX, xAxis);

   const float angleY = CCommonMath::degToRad(rotate.y);
   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   mTRMatrix = glm::rotate(mTRMatrix, angleY, yAxis);

   const float angleZ = CCommonMath::degToRad(rotate.z);
   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   mTRMatrix = glm::rotate(mTRMatrix, angleZ, zAxis);

   mModelMatrix = mTRMatrix;
}

void CGameObject::translate(const glm::vec3& translate)
{
   mTranslate = translate;

   mTRMatrix = glm::translate(mTRMatrix, translate);
   mModelMatrix = mTRMatrix;
}
