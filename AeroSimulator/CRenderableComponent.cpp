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
}

CRenderableComponent::~CRenderableComponent()
{
}

void CRenderableComponent::handleEvent(CAppEvent * pEvent)
{
   if (pEvent)
   {
      switch (pEvent->getId())
      {
      case eGeneralEvents::UPDATE_RENDERABLE: ///@todo: rename this message to set camera matrices in renderables
      {
         ///@todo: add caching later
         // Update view/projection matrices if one of them has changed
         std::shared_ptr<CCamera> pCamera;
         GCameraManager.getCamera(GCameraManager.getCurrentCameraId(), pCamera);

         const glm::mat4x4 view = pCamera->getViewMatrix();
         mRenderable.setMatrix4Param(eShaderMatrix4Params::VIEW_MATRIX, view);

         const glm::mat4x4 projection = pCamera->getProjectionMatrix();
         mRenderable.setMatrix4Param(eShaderMatrix4Params::PROJECTION_MATRIX, projection);

         // Update other camera-related stuff (up/right vector etc.)
      }
      break;
      } // switch
   }
}

CRenderable & CRenderableComponent::getRenderable()
{
   return mRenderable;
}
