#include "CComponent.h"

//using namespace AeroSimulatorEngine;

///@todo: move this file to AeroSimulatorEngine when the old CGameObject is removed

NGame::CComponent::CComponent(CGameObject * pOwner)
   : mOwner(pOwner)
{
}

NGame::CComponent::~CComponent()
{
}
