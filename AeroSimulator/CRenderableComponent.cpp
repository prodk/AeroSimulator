#include "CRenderableComponent.h"
#include "CRenderable.h"
#include "CEventManager.h"
#include "CAppEvent.h"
#include "src\shaders\CShader.h"
#include "CCameraManager.h"
#include "CCamera.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CRenderableComponent::CRenderableComponent(CGameObject * pOwner)
   : CComponent(pOwner)
   , CEventHandler()
   , mRenderable()
{
   const bool status = GEventManager.registerEvent(eGeneralEvents::UPDATE);
   if (status)
   {
      LOGGL("CRenderableComponent: UPDATE event registered: ", status);
   }
   GEventManager.attachEvent(eGeneralEvents::UPDATE, *this);
}

CRenderableComponent::~CRenderableComponent()
{
}

void CRenderableComponent::init()
{
}

void CRenderableComponent::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      switch (pEvent->getId())
      {
         ///@todo: probably rename to UPDATE_RENDERABLE
      case eGeneralEvents::UPDATE:
      {
         //LOG("CRenderableComponent: UPDATE");
         ///@todo: add caching later
         // Update view/projection matrices if one of them has changed
         std::shared_ptr<CCamera> pCamera;
         GCameraManager.getCamera(GCameraManager.getCurrentCameraId(), pCamera);

         ///@todo: use the correct matrix here, probably MVP
         const glm::mat4x4 view = pCamera->getViewMatrix();
         mRenderable.setMatrix4Param(eShaderMatrix4Params::VIEW_MATRIX, view);
         //mRenderable.setMatrix4Param(eShaderMatrix4Params::VIEW_MATRIX, pCamera->getViewMatrix());

         // Update model matrix

         // Update view/projection

         // Update other camera-related stuff
      }
      break;
      } // switch
   }
}

CRenderable & CRenderableComponent::getRenderable()
{
   return mRenderable;
}
