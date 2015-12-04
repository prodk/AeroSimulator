#include "CLand.h"
#include "CGeometry.h"
#include "CLog.h"
#include "CTexture.h"
#include "CBoundingBox.h"

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
   : mScaledTRMatrix()
   , mBoundingBox()
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

CLand::~CLand()
{
}

void CLand::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().log("\n** CLand::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

bool CLand::loadTexture(const char * fileName)
{
   return (0 != mTexture->loadDDSTexture(fileName));
}

void CLand::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   CParentGameObject::buildModelMatrix(parentTRMatrix);

   // We need to calculate the model matrix for the node
   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
   mModelMatrix = mParentTRMatrix * mScaledTRMatrix;
}

void CLand::updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt)
{
   CParentGameObject::updateTRMatrix(trMatrix, dt);

   // Don't forget to change the cached scaled TR matrix
   if (trMatrix != mParentTRMatrix)
   {
      mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
   }
}

void CLand::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   CParentGameObject::updateModelMatrix(rootModelMatrix);

   mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;
}

void CLand::setBoundingBox(std::shared_ptr<CShader>& pShader, const glm::vec3& size, const glm::vec4& color)
{
   if (pShader)
   {
      mBoundingBox.reset(new CBoundingBox());
      if (mBoundingBox)
      {
         mBoundingBox->setColor(color);
         mBoundingBox->setScale(size);
         mBoundingBox->calculateModelMatrix();

         mBoundingBox->setShadersAndBuffers(pShader);
         add(mBoundingBox.get());
      }

      buildModelMatrix(glm::mat4x4(1.0f)); // Bind children positions to the root

                                           ///@todo: debug
                                           //mBoundingBox->setVisible(false);
   }
}

const CBoundingBox * CLand::getBoundingBox() const
{
   if (mBoundingBox)
      return mBoundingBox.get();
   else
      return 0;
}
