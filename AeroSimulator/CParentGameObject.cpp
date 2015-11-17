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

CParentGameObject::CParentGameObject(const CGameObject* parent,
                                     const glm::vec3 & scale,
                                     const glm::vec3 & rotate,
                                     const glm::vec3 & translate)
   : CGameObject(parent, scale, rotate, translate)
   , mChildren()
{
}

void CParentGameObject::setupGeometry(std::shared_ptr<CGeometry>& pGeometry)
{
}

void CParentGameObject::setupShadersAndBuffers(std::shared_ptr<CShader>& pShader)
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

void CParentGameObject::updateMatrix(const glm::mat4 & parentMatrix, const glm::mat4& dynamicMatrix)
{
   const std::size_t numOfChildren = mChildren.size();
   for (std::size_t count = 0; count < numOfChildren; ++count)
   {
      mChildren[count]->updateMatrix(parentMatrix, dynamicMatrix);
   }
}

void CParentGameObject::setDynamic()
{
   const std::size_t numOfChildren = mChildren.size();
   for (std::size_t count = 0; count < numOfChildren; ++count)
   {
      mChildren[count]->setDynamic();
   }
}

glm::mat4 CParentGameObject::getChildTRMatrix(std::size_t childId) const
{
   glm::mat4 result = glm::mat4(1);

   if (childId < mChildren.size())
      return result = mChildren[childId]->getTRMatrix();

   return result;
}
