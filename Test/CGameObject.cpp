#include "CGameObject.h"
#include "CComponent.h"

///@todo: move this file to AeroSimulatorEngine when the old CGameObject is removed

///@todo: move to namespace AeroSimulatorEngine when the old CGameObject is removed
//using namespace AeroSimulatorEngine;

Game::CGameObject::CGameObject()
{
}

Game::CGameObject::~CGameObject()
{
}

Game::CComponent * Game::CGameObject::getComponent(unsigned int id)
{
   auto result = mComponents.find(id);

   return result == mComponents.end()
      ? nullptr
      : result->second.get();
}
