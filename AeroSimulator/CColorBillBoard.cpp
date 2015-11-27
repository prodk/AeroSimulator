#include "CColorBillBoard.h"
#include "CLog.h"
#include "CTexture.h"
#include "CGeometry.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat vertices[] =
   {
      0.0f, 0.0f, 0.0f, -0.5f, 0.5f,
      0.0f, 0.0f, 0.0f, -0.5f, -0.5f,
      0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
      0.0f, 0.0f, 0.0f, 0.5f, -0.5f
   };

   GLuint indices[] =
   {
      0, 1, 2, 3
   };
}

CColorBillBoard::CColorBillBoard()
{
   mTexture.reset(new CTexture());
   mGeometry.reset(new CGeometry());

   if (mGeometry)
   {
      mGeometry->setVertexBuffer(vertices);
      const int numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
      mGeometry->setNumOfVertices(numOfVertices);

      mGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mGeometry->setNumOfIndices(numOfIndices);

      mGeometry->setNumOfElementsPerVertex(3);
      mGeometry->setVertexStride(5);
   }
}

CColorBillBoard::~CColorBillBoard()
{
}

void CColorBillBoard::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().logGL("\n** CColorBillBoard::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

void CColorBillBoard::setEnvironment()
{
   glDepthMask(GL_FALSE);
}

void CColorBillBoard::resetEnvironment()
{
   glDepthMask(GL_TRUE);
}

void CColorBillBoard::add(CCompositeGameObject * child)
{
   CLog::getInstance().log("\n!!! Cannot add a child to CBillBoard because it is a leaf!!! \n");
}

void CColorBillBoard::traverse(std::vector<CCompositeGameObject*>& tree)
{
   // No children -nothing to do here
}

void CColorBillBoard::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   mParentTRMatrix = parentTRMatrix;
   calculateTRMatrix();

   const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make it a member
   mModelMatrix = mParentTRMatrix * scaledTRMatrix;

   // For the leaf cache the product of the parent by the scaled TR
   mParentByLocalTRMatrix = mModelMatrix;
}

void CColorBillBoard::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix; ///@todo: probably remove this member as only the cached value is really used.
      const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make this a member value
      mParentByLocalTRMatrix = mParentTRMatrix * scaledTRMatrix;
   }
}

void CColorBillBoard::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   mModelMatrix = rootModelMatrix * mParentByLocalTRMatrix;
}
