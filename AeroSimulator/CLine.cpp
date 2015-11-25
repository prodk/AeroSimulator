#include "CLine.h"
#include "CShader.h"
#include "CLog.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CLine::CLine() : CCompositeGameObject()
{
   mIsLeaf = true;
   mDrawWithLines = true;
}

CLine::~CLine()
{
}

void CLine::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().log("\n** CLine::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

void CLine::add(CCompositeGameObject * child)
{
   CLog::getInstance().log("\n!!! Cannot add a child to CCube because it is a leaf!!! \n");
}

void CLine::traverse(std::vector<CCompositeGameObject*>& tree)
{
   // No children -nothing to do here
}

void CLine::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   mParentTRMatrix = parentTRMatrix;
   calculateTRMatrix();

   const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make it a member
   mModelMatrix = mParentTRMatrix * scaledTRMatrix;

   // For the leaf cache the product of the parent by the scaled TR
   mParentByLocalTRMatrix = mModelMatrix;
}

void CLine::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix; ///@todo: probably remove this member as only the cached value is really used.
      const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make this a member value
      mParentByLocalTRMatrix = mParentTRMatrix * scaledTRMatrix;
   }
}

void CLine::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   mModelMatrix = rootModelMatrix * mParentByLocalTRMatrix;
}
