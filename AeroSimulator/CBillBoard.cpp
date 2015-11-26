#include "CBillBoard.h"
#include "CGeometry.h"
#include "CLog.h"
#include "CTexture.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat vertices[] =
   {
      0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f, -0.5f,
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.5f,
      0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, -0.5f
   };

   GLuint indices[] =
   {
      0, 1, 2, 3
   };
}

CBillBoard::CBillBoard()
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

      mGeometry->setNumOfElementsPerVertex(3); ///@todo: probably remove this
      mGeometry->setVertexStride(7); // 3 coords + 2 tex coords + 2 squad
   }
}

CBillBoard::~CBillBoard()
{
}

void CBillBoard::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().logGL("\n** CBillBoard::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

void CBillBoard::setEnvironment()
{
   glDepthMask(GL_FALSE);
}

void CBillBoard::resetEnvironment()
{
   glDepthMask(GL_TRUE);
}

bool CBillBoard::loadTexture(const char * fileName)
{
   return (0 != mTexture->loadDDSTexture(fileName));
}

void CBillBoard::add(CCompositeGameObject * child)
{
   CLog::getInstance().log("\n!!! Cannot add a child to CBillBoard because it is a leaf!!! \n");
}

void CBillBoard::traverse(std::vector<CCompositeGameObject*>& tree)
{
   // No children -nothing to do here
}

void CBillBoard::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   mParentTRMatrix = parentTRMatrix;
   calculateTRMatrix();

   const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make it a member
   mModelMatrix = mParentTRMatrix * scaledTRMatrix;

   // For the leaf cache the product of the parent by the scaled TR
   mParentByLocalTRMatrix = mModelMatrix;
}

void CBillBoard::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix; ///@todo: probably remove this member as only the cached value is really used.
      const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make this a member value
      mParentByLocalTRMatrix = mParentTRMatrix * scaledTRMatrix;
   }
}

void CBillBoard::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   mModelMatrix = rootModelMatrix * mParentByLocalTRMatrix;
}
