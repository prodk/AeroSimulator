#include "CTransformComponent.h"
#include "CGameObject.h"
#include "CEventManager.h"
#include "CRenderableComponent.h"
#include "CCameraComponent.h"

using namespace AeroSimulatorEngine;

CTransformComponent::CTransformComponent(CGameObject* pOwner)
   : CComponent(pOwner)
   , mTransform()
   , mParentTrMatrix()
   , mChildren()
{
}

CTransformComponent::~CTransformComponent()
{
}

void CTransformComponent::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      switch (pEvent->getId())
      {
      case eGeneralEvents::UPDATE_TRANSFORM:
         update();
         break;
      }
   }
}

void CTransformComponent::addChild(CTransformComponent* pChild)
{
   if (0 != pChild)
      mChildren.push_back(pChild);
}

bool CTransformComponent::removeChild(CTransformComponent* pChild)
{
   bool res = false;
   const auto child = std::find(mChildren.begin(), mChildren.end(), pChild);
   if (mChildren.end() != child)
   {
      mChildren.erase(child);
      res = true;
   }
   return res;
}

///@note: this must be called after all movements/rotations etc. as here we set the model matrix
void CTransformComponent::update()
{
   updateParentMatrix(mParentTrMatrix);

   CGameObject& owner = *getOwner();
   CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(owner);
   if (pRenderableComp) {
      pRenderableComp->getRenderable().setMatrix4Param(eShaderMatrix4Params::MODEL_MATRIX, mParentTrMatrix * mTransform.getModelMatrix());
   }
}

void CTransformComponent::updateParentMatrix(const glm::mat4& parent)
{
   mParentTrMatrix = parent;
   for (auto c : mChildren) {
      if (nullptr != c)
      {
         const glm::mat4& tr = mTransform.getTranslateRotateMatrix();
         c->updateParentMatrix(mParentTrMatrix * tr);
      }
   }
}
