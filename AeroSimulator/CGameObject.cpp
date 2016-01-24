#include "CGameObject.h"
#include "CComponent.h"

using namespace AeroSimulatorEngine;

CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
}

CComponent * CGameObject::getComponent(const unsigned int id) const
{
   const auto result = mComponents.find(id);

   return result == mComponents.end()
      ? nullptr
      : result->second.get();
}
