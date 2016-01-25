#include "CRenderableComponent.h"
#include "../CRenderable.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CRenderableComponent::CRenderableComponent(CGameObject * pOwner)
   : CComponent(pOwner)
   , mRenderable(new AeroSimulatorEngine::CRenderable())
{
   assert(mRenderable);
}

CRenderableComponent::~CRenderableComponent()
{
}

void CRenderableComponent::init()
{
}
