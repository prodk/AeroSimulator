#include "CRenderableComponent.h"
#include "CRenderable.h"
#include "CEventManager.h"
#include "CAppEvent.h"
#include "src\shaders\CShader.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CRenderableComponent::CRenderableComponent(CGameObject * pOwner)
   : CComponent(pOwner)
   , mRenderable()
{
   const bool status = GEventManager.registerEvent(eGeneralEvents::UPDATE);
   if (status)
   {
      LOGGL("CRenderableComponent: UPDATE event registered: ", status);
      GEventManager.attachEvent(eGeneralEvents::UPDATE, *this);
   }
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
      case eGeneralEvents::UPDATE:
      {
         //LOG("CRenderableComponent: UPDATE");
         ///@todo: add caching later
         // Update view/projection matrices if one of them has changed

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
