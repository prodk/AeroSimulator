#include "CGameObject.h"
#include "CComponent.h"
#include "CRenderableComponent.h"
#include "CRenderable.h"
#include "../src/shaders/CShader.h"
#include "CTransformComponent.h"
#include "CMovementComponent.h"
#include "CCameraComponent.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CGameObject::CGameObject()
   : mComponents()
   , mId(-1)
   , mType(-1)
   , mFrameDt(0.0f)
   , mChildren()
{
}

CGameObject::CGameObject(const int id, const int type)
   : mComponents()
   , mId(id)
   , mType(type)
   , mFrameDt(0.0f)
   , mChildren()
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::addChild(std::shared_ptr<CGameObject>& pChild)
{
   if (nullptr != pChild)
   {
      mChildren.push_back(pChild);
      ///@todo: here the translateRotate matrix should be updated for the first time somehow at least for CCameraComponent
      ///@todo: probably send a message and set the camera to a special state: parentChanged, then rest it after the update
   }
}

bool CGameObject::removeChild(std::shared_ptr<CGameObject>& pChild)
{
   bool result = false;
   const auto child = std::find(mChildren.begin(), mChildren.end(), pChild);
   if (mChildren.end() != child)
   {
      mChildren.erase(child);
      result = true;
   }

   return result;
}

CComponent * CGameObject::getComponent(const unsigned int id) const
{
   const auto result = mComponents.find(id);

   //CComponent* ptr = result->second.get();

   return result == mComponents.end()
      ? nullptr
      : result->second.get();
}

bool CGameObject::addRenderableComponent(tShaderPtr & pShader, const std::string & textureFilePath,
                                         const SGeometryData& geometryData, const std::vector<int>& events, const char * msg)
{
   bool result = false;

   ///@todo: handle a situation when we have a texture shader but texture file path is incorrect
   if (addComponent<CRenderableComponent>())
   {
      LOG(msg, " addRenderableComponent() success");

      getRenderable().setGeometry(geometryData);
      getRenderable().setShader(pShader);
      if (!textureFilePath.empty())
      {
         getRenderable().createAndLoadTexture(MAIN_TEXTURE, textureFilePath.c_str(), DDS);
      }

      registerEvents(events, msg);
      attachEvents<CRenderableComponent>(events, msg);

      result = true;
   }
   else
   {
      LOG(msg, " addRenderableComponent() failed");
   }

   return result;
}

bool CGameObject::addTransformComponent(const std::vector<int>& events, const char * msg)
{
   bool result = false;

   if (addComponent<CTransformComponent>())
   {
      LOG(msg, " addTransformComponent() success");

      registerEvents(events, msg);
      attachEvents<CTransformComponent>(events, msg);

      result = true;
   }
   else
   {
      LOG(msg, " addTransformComponent() failed");
   }

   return result;
}

bool CGameObject::addMovementComponent(const std::vector<int>& events, const char * msg)
{
   bool result = false;

   if (addComponent<CMovementComponent>())
   {
      LOG(msg, " addMovementComponent() success");

      registerEvents(events, msg);
      attachEvents<CMovementComponent>(events, msg);

      result = true;
   }

   return result;
}

void CGameObject::registerEvents(const std::vector<int>& events, const char * msg) const
{
   for (auto e : events)
   {
      if (GEventManager.registerEvent(e))
      {
         LOG(msg, " registerEvents(): ", e);
      }
   }
}

CRenderable & CGameObject::getRenderable()
{
   CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(*this);
   assert(pRenderableComp);

   return pRenderableComp->getRenderable();
}

///@note: important that this method is called before updating the final model matrix
bool CGameObject::updateChildrensMatrix(const glm::mat4& m)
{
   bool result = false;

   CTransformComponent* pTransformComponent = componentCast<CTransformComponent>(*this);
   if (pTransformComponent)
   {
      updateChildrensMatrix<CTransformComponent>(m, pTransformComponent);

      result = true;
   }
   else
   {
      CCameraComponent* pCameraComponent = componentCast<CCameraComponent>(*this);
      if (pCameraComponent)
      {
         updateChildrensMatrix<CCameraComponent>(m, pCameraComponent);
         result = true;
      }
   }

   //if (pComponent)
   //{
   //   result = true;
   //   CTransform& transform = pComponent->getTransform();
   //   pComponent->setParentMatrix(m);
   //   const glm::mat4 translateRotateWithParent = m * transform.getTranslateRotateMatrix();
   //   transform.setTranslateRotateMatrix(translateRotateWithParent);

   //   for (auto child : mChildren)
   //   {
   //      if (child)
   //      {
   //         //child->setParentMatrix(translateRotateWithParent);
   //         child->updateChildrensMatrix(translateRotateWithParent);
   //      }
   //   }
   //}

   return result;
}

///@todo:remove this method
void CGameObject::setParentMatrix(const glm::mat4& m)
{
   CTransformComponent* pComponent = componentCast<CTransformComponent>(*this);
   if (0 == pComponent)
   {
      //bool has = hasComponent<CCameraComponent>();
      //CCameraComponent* pComponent = componentCast<CCameraComponent>(*this);
      CCameraComponent* pComponent = getComponent<CCameraComponent>();
   }

   if (pComponent)
   {
      pComponent->setParentMatrix(m);
   }
}
