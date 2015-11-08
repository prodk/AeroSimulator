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

void CParentGameObject::setupGeometry()
{
}

void CParentGameObject::setupVBO()
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
      if (mChildren[count]->isLeaf())
      {
         tree.push_back(mChildren[count]);
      }
      else
      {
         mChildren[count]->traverse(tree);
      }
   }
}
