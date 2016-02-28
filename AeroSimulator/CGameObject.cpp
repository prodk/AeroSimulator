#include "CGameObject.h"
#include "CComponent.h"
#include "CRenderableComponent.h"
#include "CRenderable.h"
#include "../src/shaders/CShader.h"
#include "CTransformComponent.h"
#include "CMovementComponent.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CGameObject::CGameObject()
   : mComponents()
   , mId(-1)
   , mType(-1)
   , mFrameDt(0.0f)
{
}

CGameObject::CGameObject(const int id, const int type)
   : mComponents()
   , mId(id)
   , mType(type)
   , mFrameDt(0.0f)
{
}

CGameObject::~CGameObject()
{
}

CComponent * CGameObject::getComponent(const unsigned int id) const
{
   const auto result = mComponents.find(id);

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

