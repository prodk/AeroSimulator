#include "CComponent.h"

using namespace AeroSimulatorEngine;

CComponent::CComponent(CGameObject * pOwner)
   : mOwner(pOwner)
{
}

CComponent::~CComponent()
{
}
