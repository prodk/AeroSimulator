#include "CBillBoard.h"
#include "CGeometry.h"
#include "CTexture.h"
#include "CLog.h"
#include "CBoundingBox.h"

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
   : mScaledTRMatrix()
   , mBoundingBox()
{
   mTexture.reset(new CTexture());
   mGeometry.reset(new CGeometry());

   assert(mTexture);
   assert(mGeometry);

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
   const bool result = (0 != mTexture->loadDDSTexture(fileName));

   if (result && (mTexture->getWidth() != mTexture->getHeight()))
   {
      glGenerateTextureMipmap(mTexture->getId());
      CLog::getInstance().log("CBillBoard::loadTexture(): generating mipmaps for non-square texture, height: ",
         mTexture->getHeight());
   }

   return result;
}

void CBillBoard::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().log("\n** CCube::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

void CBillBoard::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   CParentGameObject::buildModelMatrix(parentTRMatrix);

   // We need to calculate the model matrix for the node
   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
   mModelMatrix = mParentTRMatrix * mScaledTRMatrix;
}

void CBillBoard::updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt)
{
   CParentGameObject::updateTRMatrix(trMatrix, dt);

   // Don't forget to change the cached scaled TR matrix
   if (trMatrix != mParentTRMatrix)
   {
      mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
   }
}

void CBillBoard::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   CParentGameObject::updateModelMatrix(rootModelMatrix);

   mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;
}

void CBillBoard::setBoundingBox(std::shared_ptr<CShader>& pShader, const glm::vec4& color, const glm::vec3& size)
{
   if (pShader)
   {
      mBoundingBox.reset(new CBoundingBox());
      if (mBoundingBox)
      {
         mBoundingBox->setColor(color);
         mBoundingBox->setScale(glm::vec3(mBillboardWidth, mBillboardHeight, mBillboardWidth));
         mBoundingBox->calculateModelMatrix();

         mBoundingBox->setShadersAndBuffers(pShader);
         add(mBoundingBox.get());
      }

      buildModelMatrix(glm::mat4x4(1.0f)); // Bind children positions to the root
   }
}

const CBoundingBox * CBillBoard::getBoundingBox() const
{
   if (mBoundingBox)
      return mBoundingBox.get();
   else
      return 0;
}

void CBillBoard::setVisible(const bool visible)
{
   mIsVisible = visible;
   if (mBoundingBox)
      mBoundingBox->setVisible(visible);
}
