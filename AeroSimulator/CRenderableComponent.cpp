#include "CRenderableComponent.h"
#include "CRenderable.h"
#include "src\shaders\CShader.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CRenderableComponent::CRenderableComponent(CGameObject * pOwner)
   : CComponent(pOwner)
   , mRenderable()
{
}

CRenderableComponent::~CRenderableComponent()
{
}

void CRenderableComponent::init()
{
}

CRenderable & CRenderableComponent::getRenderable()
{
   return mRenderable;
}
