#include "CTransformComponent.h"

using namespace AeroSimulatorEngine;

CTransformComponent::CTransformComponent(CGameObject* pOwner)
   : CComponent(pOwner)
   , mTransform()
{
}

CTransformComponent::~CTransformComponent()
{
}
