#include "CLeafCompositeGameObject.h"
#include "CLog.h"
#include "CTexture.h"
#include "CGeometry.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CLeafCompositeGameObject::CLeafCompositeGameObject()
{
}

CLeafCompositeGameObject::~CLeafCompositeGameObject()
{
}

void CLeafCompositeGameObject::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().logGL("\n** CLeafCompositeGO::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

void CLeafCompositeGameObject::add(CCompositeGameObject * child)
{
   CLog::getInstance().log("\n!!! Cannot add a child to CBillBoard because it is a leaf!!! \n");
}

void CLeafCompositeGameObject::traverse(std::vector<CCompositeGameObject*>& tree)
{
   // No children -nothing to do here
}

void CLeafCompositeGameObject::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   mParentTRMatrix = parentTRMatrix;
   calculateTRMatrix();

   const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make it a member
   mModelMatrix = mParentTRMatrix * scaledTRMatrix;

   // For the leaf cache the product of the parent by the scaled TR
   mParentByLocalTRMatrix = mModelMatrix;
}

void CLeafCompositeGameObject::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix; ///@todo: probably remove this member as only the cached value is really used.
      const glm::mat4x4 scaledTRMatrix = glm::scale(mTRMatrix, mScale); ///@todo: make this a member value
      mParentByLocalTRMatrix = mParentTRMatrix * scaledTRMatrix;
   }
}

void CLeafCompositeGameObject::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   mModelMatrix = rootModelMatrix * mParentByLocalTRMatrix;
}
