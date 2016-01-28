#include "CLand.h"
#include "../CGeometry.h"
#include "../CLog.h"
#include "../CTexture.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat vertices[] =
   {
     -0.5f, 0.0f, -0.5f, 0.0f, 1.0f,
      -0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
      0.5f, 0.0f, -0.5f, 1.0f, 1.0f,
      0.5f, 0.0f, 0.5f, 1.0f, 0.0f
   };

   GLuint indices[] =
   {
      0, 1, 2, 3
   };
}

CLand::CLand()
   //: mScaledTRMatrix()
   //, mNumOfTiles(glm::vec2(1, 1))
{
   //setRepeatTexture(true);
   //mTexture.reset(new CTexture());
   //mGeometry.reset(new CGeometry());
}

CLand::~CLand()
{
}

//void CLand::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
//{
   //if (mGeometry)
   //{
   //   const int numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
   //   // Modify number of tiles
   //   for (std::size_t count = 0; count < numOfVertices; ++count)
   //   {
   //      if ((count % 5) == 3)
   //      {
   //         vertices[count] *= mNumOfTiles.x;
   //      }
   //      if ((count % 5) == 4)
   //      {
   //         vertices[count] *= mNumOfTiles.y;
   //      }
   //   }

   //   mGeometry->setVertexBuffer(vertices);
   //   mGeometry->setNumOfVertices(numOfVertices);

   //   mGeometry->setIndexBuffer(indices);
   //   const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
   //   mGeometry->setNumOfIndices(numOfIndices);

   //   mGeometry->setNumOfElementsPerVertex(3);
   //   mGeometry->setVertexStride(5);

   //   CLog::getInstance().log("* CLand::setupShadersAndBuffers(): Land's geometry has been setup **");
   //}

   //CGameObject::setShadersAndBuffers(pShader);
//}

//bool CLand::loadTexture(const char * fileName)
//{
//   return (0 != mTexture->loadDDSTexture(fileName));
//}
//
//void CLand::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
//{
//   CParentGameObject::buildModelMatrix(parentTRMatrix);
//
//   // We need to calculate the model matrix for the node
//   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
//   mModelMatrix = mParentTRMatrix * mScaledTRMatrix;
//}
//
//void CLand::updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt)
//{
//   CParentGameObject::updateTRMatrix(trMatrix, dt);
//
//   // Don't forget to change the cached scaled TR matrix
//   if (trMatrix != mParentTRMatrix)
//   {
//      mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
//   }
//}
//
//void CLand::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
//{
//   CParentGameObject::updateModelMatrix(rootModelMatrix);
//
//   mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;
//}

//void CLand::setNumOfTiles(const GLint x, const GLint y)
//{
  /* mNumOfTiles.x = (x > 0) ? x : 1;
   mNumOfTiles.y = (y > 0) ? y : 1;*/
//}
