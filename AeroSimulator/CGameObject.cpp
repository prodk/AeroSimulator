#include "CGameObject.h"
#include "CSimpleShader.h" ///@todo: use another header when DEG_TO_RAD is moved to other place

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CGameObject::CGameObject()
   : CRenderable()
   , mScale()
   , mRotate()
   , mTranslate()
   , mIsLeaf(false)
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
   //, mModelMatrix()
   , mScale(scale)
   , mRotate(rotate)
   , mTranslate(translate)
   , mIsLeaf(false)
{
}

void CGameObject::resetModelMatrix(const glm::mat4& matrix)
{
   mModelMatrix = matrix;
}

void CGameObject::scale(const glm::vec3& scale)
{
   mScale = scale;
   mModelMatrix = glm::scale(mModelMatrix, scale);
}

void CGameObject::rotate(const glm::vec3& rotate)
{
   mRotate = rotate;

   const float angleX = DEG_TO_RAD * rotate.x;
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   mModelMatrix = glm::rotate(mModelMatrix, angleX, xAxis);

   const float angleY = DEG_TO_RAD * rotate.y;
   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   mModelMatrix = glm::rotate(mModelMatrix, angleY, yAxis);

   const float angleZ = DEG_TO_RAD * rotate.z;
   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   mModelMatrix = glm::rotate(mModelMatrix, angleZ, zAxis);
}

void CGameObject::translate(const glm::vec3& translate)
{
   mTranslate = translate;

   mModelMatrix = glm::translate(mModelMatrix, translate);
}
