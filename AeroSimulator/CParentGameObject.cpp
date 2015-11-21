#include "CParentGameObject.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

CParentGameObject::CParentGameObject()
   : CGameObject()
   , mChildren()
{
}

CParentGameObject::~CParentGameObject()
{
}

CParentGameObject::CParentGameObject(const glm::vec3 & scale,
                                     const glm::vec3 & rotate,
                                     const glm::vec3 & translate)
   : CGameObject(scale, rotate, translate)
   , mChildren()
{
   //mCanBeRendered = false;
}

//void CParentGameObject::setupGeometry(std::shared_ptr<CGeometry>& pGeometry)
//{
//}

void CParentGameObject::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
}

void CParentGameObject::add(CGameObject * child)
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

void CParentGameObject::traverse(std::vector<CGameObject*>& tree)
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
   ///@todo: add caching of the parentTR*mTR product somewhere
   mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;

   ///@todo: probably calculateTR matrix here and cache its product with the parent matrix (which cameas an argument to this method)
   // then use the cached value as an argument too.
   for (auto * pChild : mChildren)
   {
      if (pChild)
      {
         pChild->buildModelMatrix(mParentByLocalTRMatrix); ///@todo: add the cached product as an argument here
      }
   }
}

void CParentGameObject::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   ///@todo: we can change the mTRMatrix here such that there is motion of the parent (use for the propeller)

   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix;
      ///@todo: update the cached product of the parentTR by the current localTR here;
      mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;
   }

   for (auto * pChild : mChildren)
   {
      if (pChild)
      {
         pChild->updateTRMatrix(mParentByLocalTRMatrix); ///avoid recalculation on every frame
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

//void CParentGameObject::updateMatrix(const glm::mat4 & parentMatrix, const glm::mat4& dynamicMatrix)
//{
//   ///@todo: debug
//   const std::size_t numOfChildren = mChildren.size();
//   for (std::size_t count = 0; count < numOfChildren; ++count)
//   {
//      mChildren[count]->updateMatrix(parentMatrix, dynamicMatrix);
//   }
//}
//
//void CParentGameObject::setDynamic()
//{
//   const std::size_t numOfChildren = mChildren.size();
//   for (std::size_t count = 0; count < numOfChildren; ++count)
//   {
//      mChildren[count]->setDynamic();
//   }
//}
//
//glm::mat4 CParentGameObject::getChildTRMatrix(std::size_t childId) const
//{
//   glm::mat4 result = glm::mat4(1);
//
//   if (childId < mChildren.size())
//      return result = mChildren[childId]->getTRMatrix();
//
//   return result;
//}
