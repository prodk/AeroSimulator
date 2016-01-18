#include "CComponent.h"

//using namespace AeroSimulatorEngine;

///@todo: move this file to AeroSimulatorEngine when the old CGameObject is removed

Game::CComponent::CComponent(CGameObject * pOwner)
   : mOwner(pOwner)
{
}

Game::CComponent::~CComponent()
{
}
