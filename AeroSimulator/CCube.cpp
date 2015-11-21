#include "CCube.h"
#include "CGeometry.h"
//#include "CSimpleShader.h"
#include "CShader.h"
#include "CLog.h"
//#include "CCommonMath.h"

#include <cassert>

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

///@todo: reconsider these values and their usage
const int CCube::mNumOfElementsPerVertex = 3; // 3 coordinates/color components per vertex
const int CCube::mStride = 6;                 // stride of 6 for 3 coordinates and 3 colors

CCube::CCube()
   : CGameObject()
{
   mIsLeaf = true;

   mScale = glm::vec3(1.0f, 1.0f, 1.0f);

   CLog::getInstance().log("* CCube::CCube() default: success.");
}

CCube::~CCube()
{
   //mGeometry.reset();
   //mShader.reset();
}

CCube::CCube(const CGameObject* parent,
             const glm::vec3 & scale,
             const glm::vec3 & rotate,
             const glm::vec3 & translate)
   : CGameObject(scale, rotate, translate)
{
   mIsLeaf = true;

   //// Set transforms
   //glm::mat4 myModel = glm::mat4(1.0f);

   //// TRS
   //// scale
   //myModel = glm::scale(myModel, scale);

   //// rotate
   //const float angleX = CCommonMath::degToRad(rotate.x);
   //glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   //myModel = glm::rotate(myModel, angleX, xAxis);

   //const float angleY = CCommonMath::degToRad(rotate.y);
   //glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   //myModel = glm::rotate(myModel, angleY, yAxis);

   //const float angleZ = CCommonMath::degToRad(rotate.z);
   //glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   //myModel = glm::rotate(myModel, angleZ, zAxis);

   //// translate
   //myModel = glm::translate(myModel, translate);

   /////@todo: probably myltiply by the parent matrix
   //glm::mat4 parentModelMatrix = glm::mat4(1.0f);
   //if (parent)
   //{
   //   parentModelMatrix = parent->getModelMatrix();
   //}

   //mModelMatrix = parentModelMatrix * myModel;

   CLog::getInstance().log("* CCube::CCube() non-default: success.");
}

//void CCube::setupGeometry(std::shared_ptr<CGeometry>& pGeometry)
//{
//   mGeometry = pGeometry;
//}

void CCube::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().logGL("\n** CCube::setupShadersAndBuffers() **");

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

void CCube::add(CGameObject * child)
{
   CLog::getInstance().log("\n!!! Cannot add a child to CCube because it is a leaf!!! \n");
}

void CCube::traverse(std::vector<CGameObject*>& tree)
{
   // No children -nothing to do here
}

///@todo: add a cached parent matrices as an argument here
void CCube::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   mParentTRMatrix = parentTRMatrix;
   calculateTRMatrix();

   const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make it a member
   mModelMatrix = mParentTRMatrix * scaledTRMatrix;

   // For the leaf cach the product of the parent by the scaled TR
   mParentByLocalTRMatrix = mModelMatrix;
}

void CCube::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix; ///@todo: probably remove this member as only the cached value is really used.
      const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make this a member value
      mParentByLocalTRMatrix = mParentTRMatrix * scaledTRMatrix;
   }
}

void CCube::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   mModelMatrix = rootModelMatrix * mParentByLocalTRMatrix;
}

//void CCube::updateMatrix(const glm::mat4 & parentMatrix, const glm::mat4& dynamicMatrix)
//{
//   glm::mat4 scaledTRm = glm::scale(mTRMatrix, mScale);
//   if (isDynamic())
//   {
//      mModelMatrix = parentMatrix * mParentModelMatrix * dynamicMatrix * scaledTRm;
//   }
//   else
//      mModelMatrix = parentMatrix * mParentModelMatrix * scaledTRm;
//   //mModelMatrix = parentMatrix * scaledTRm;
//}
