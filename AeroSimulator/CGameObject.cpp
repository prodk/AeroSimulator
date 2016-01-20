#include "CGameObject.h"
#include "CComponent.h"

///@todo: move this file to AeroSimulatorEngine when the old CGameObject is removed

///@todo: move to namespace AeroSimulatorEngine when the old CGameObject is removed
using namespace AeroSimulatorEngine;

CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
}

CComponent * CGameObject::getComponent(unsigned int id)
{
   auto result = mComponents.find(id);

   return result == mComponents.end()
      ? nullptr
      : result->second.get();
}
