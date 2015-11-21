#include "CParentGameObject.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

CParentGameObject::CParentGameObject()
   : CCompositeGameObject()
   , mChildren()
{
}

CParentGameObject::~CParentGameObject()
{
}

CParentGameObject::CParentGameObject(const glm::vec3 & scale,
                                     const glm::vec3 & rotate,
                                     const glm::vec3 & translate)
   : CCompositeGameObject(scale, rotate, translate)
   , mChildren()
{
}

void CParentGameObject::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
}

void CParentGameObject::add(CCompositeGameObject * child)
{
   if (nullptr != child)
   {
         mChildren.push_back(child);
   }
   else
   {
      CLog::getInstance().log("\n!!! CParentGameObject::add(): child nullptr!!!\n");
   }
}

void CParentGameObject::traverse(std::vector<CCompositeGameObject*>& tree)
{
   const std::size_t numOfChildren = mChildren.size();
   for (std::size_t count = 0; count < numOfChildren; ++count)
   {
      tree.push_back(mChildren[count]);
      if (!mChildren[count]->isLeaf())
      {
         mChildren[count]->traverse(tree);
      }
   }
}

void CParentGameObject::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   mParentTRMatrix = parentTRMatrix;
   calculateTRMatrix();
   mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;

   for (auto * pChild : mChildren)
   {
      if (pChild)
      {
         pChild->buildModelMatrix(mParentByLocalTRMatrix);
      }
   }
}

void CParentGameObject::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix;
      mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;
   }

   for (auto * pChild : mChildren)
   {
      if (pChild)
      {
         pChild->updateTRMatrix(mParentByLocalTRMatrix); /// Avoid recalculation on every frame
      }
   }
}

void CParentGameObject::updateModelMatrix(const glm::mat4x4 & modelMatrix)
{
   // A parent node does not have a valid model matrix
   for (auto * pChild : mChildren)
   {
      if (pChild)
      {
         pChild->updateModelMatrix(modelMatrix);
      }
   }
}
