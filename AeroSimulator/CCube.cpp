#include "CCube.h"
#include "CGeometry.h"
#include "CShader.h"
#include "CLog.h"

#include <cassert>

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

///@todo: reconsider these values and their usage
const int CCube::mNumOfElementsPerVertex = 3; // 3 coordinates/color components per vertex
const int CCube::mStride = 6;                 // stride of 6 for 3 coordinates and 3 colors

CCube::CCube() : CParentGameObject()
{
   //mIsLeaf = true;

   mScale = glm::vec3(1.0f, 1.0f, 1.0f);

   CLog::getInstance().log("* CCube::CCube() default: success.");
}

CCube::~CCube()
{
   mGeometry.reset();
   mShader.reset();
}

CCube::CCube(const glm::vec3 & scale,
             const glm::vec3 & rotate,
             const glm::vec3 & translate)
   : CParentGameObject(scale, rotate, translate)
{
   mIsLeaf = true;

   CLog::getInstance().log("* CCube::CCube() non-default: success.");
}

void CCube::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().log("\n** CCube::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

void CCube::add(CCompositeGameObject * child)
{
   CLog::getInstance().log("\n!!! Cannot add a child to CCube because it is a leaf!!! \n");
}

//void CCube::traverse(std::vector<CCompositeGameObject*>& tree)
//{
//   // No children -nothing to do here
//}

void CCube::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   ///@todo: think about caching of the matrixces!
   mParentTRMatrix = parentTRMatrix;
   calculateTRMatrix();
   mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;

   ///@todo: probably call the parent version and then calculate the model matrix
   // For the cube we need to calculate the model matrix
   const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make it a member
   mModelMatrix = mParentTRMatrix * scaledTRMatrix;

   for (auto * pChild : mChildren)
   {
      if (pChild)
      {
         pChild->buildModelMatrix(mParentByLocalTRMatrix);
      }
   }

   // For the leaf cache the product of the parent by the scaled TR
   //mParentByLocalTRMatrix = mModelMatrix;
}

//void CCube::updateTRMatrix(const glm::mat4x4 & trMatrix)
//{
//   if (trMatrix != mParentTRMatrix)
//   {
//      mParentTRMatrix = trMatrix; ///@todo: probably remove this member as only the cached value is really used.
//      const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make this a member value
//      mParentByLocalTRMatrix = mParentTRMatrix * scaledTRMatrix;
//   }
//}

void CCube::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   ///@todo: probably call the CParentGameObject version and then calculate the model matrix

   const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make it a member

   //mModelMatrix = rootModelMatrix * mParentByLocalTRMatrix;
   mModelMatrix = rootModelMatrix * mParentTRMatrix * scaledTRMatrix;
}
