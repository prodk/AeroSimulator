#include "CGameObject.h"
#include "CCommonMath.h"
#include "CGeometry.h"
#include "CShader.h"
#include "CLog.h"

#include <gl/GL.h>

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CGameObject::CGameObject()
   : CRenderable()
   , mScale(glm::vec3(1.0f, 1.0f, 1.0f))
   , mRotate(glm::vec3(0.0f, 0.0f, 0.0f))
   , mTranslate(glm::vec3(0.0f, 0.0f, 0.0f))
   , mTRMatrix()
   , mParentTRMatrix()
   , mParentByLocalTRMatrix()
   , mIsLeaf(false)
{
}

CGameObject::~CGameObject()
{
}

CGameObject::CGameObject(const glm::vec3 & scale,
                         const glm::vec3 & rotate,
                         const glm::vec3 & translate)
   : CRenderable()
   , mScale(scale)
   , mRotate(rotate)
   , mTranslate(translate)
   , mParentTRMatrix()
   , mIsLeaf(false)
{
}

void CGameObject::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   // Shader setup
   assert(pShader);
   mShader = pShader;

   if (mGeometry)
   {
      // VBO
      glGenBuffers(1, &mVboId);
      glBindBuffer(GL_ARRAY_BUFFER, mVboId);
      CLog::getInstance().logGL("* glBindBuffer() VBO: ");

      GLuint* data = static_cast<GLuint*>(mGeometry->getVertexBuffer());
      glBufferData(GL_ARRAY_BUFFER, mGeometry->getNumOfVertices()* sizeof(GLuint), data, GL_STATIC_DRAW);

      // Index buffer
      glGenBuffers(1, &mIboId);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboId);
      CLog::getInstance().logGL("* glBindBuffer() index buffer: ");

      GLuint* indices = (GLuint*)mGeometry->getIndexBuffer();
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, mGeometry->getNumOfIndices()* sizeof(GLuint), indices, GL_STATIC_DRAW);

      // Reset VBOs
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }
}

glm::mat4 CGameObject::getTRMatrix() const
{
   return mTRMatrix;
}

void CGameObject::calculateTRMatrix()
{
   // To get a TRS sequence of matrices, act as follows: translate, rotate, scale
   // 1) translate
   mTRMatrix = glm::mat4x4(1);
   mTRMatrix = glm::translate(mTRMatrix, mTranslate);

   // 2) rotate
   // x axis
   const float angleX = CCommonMath::degToRad(mRotate.x);
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   mTRMatrix = glm::rotate(mTRMatrix, angleX, xAxis);

   // y axis
   const float angleY = CCommonMath::degToRad(mRotate.y);
   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   mTRMatrix = glm::rotate(mTRMatrix, angleY, yAxis);

   // z axis
   const float angleZ = CCommonMath::degToRad(mRotate.z);
   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   mTRMatrix = glm::rotate(mTRMatrix, angleZ, zAxis);

   // Scale is used only for the model matrix
}

void CGameObject::calculateModelMatrix()
{
   calculateTRMatrix();

   mModelMatrix = glm::scale(mTRMatrix, mScale);
}

void CGameObject::scale(const glm::vec3& scale)
{
   mScale = scale;
   mModelMatrix = glm::scale(mModelMatrix, scale);
}

void CGameObject::translate(const glm::vec3& translate)
{
   mTranslate = translate;

   mTRMatrix = glm::translate(mTRMatrix, translate);
   mModelMatrix = mTRMatrix;
}
