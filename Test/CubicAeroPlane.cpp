#include "CCubicAeroPlane.h"
#include "../CTransform.h"

using namespace AeroSimulatorEngine;

CCubicAeroPlane::CCubicAeroPlane(const int id, const int type)
   : CGameObject(id, type)
   , mCubes()
{
   std::vector<int> moveEvents;
   moveEvents.push_back(eGeneralEvents::MOVE);
   (void)CGameObject::addMovementComponent(moveEvents, "* CCubicAeroPlane: ");

   addCubes();
}

CCubicAeroPlane::~CCubicAeroPlane()
{
}

void CCubicAeroPlane::move()
{
}

void CCubicAeroPlane::addCubes()
{
   CTransform transform;

   const std::size_t cabineId = mCubes.size();
   const glm::vec4 cabineColor(0.0f, 0.0f, 1.0f, 1.0f);
   //transform.setTranslate(glm::vec3(0.0f, 29.75f, 0.0f));
   transform.setTranslate(glm::vec3(0.0f, 35.f, 0.0f));
   transform.setScale(glm::vec3(0.5f, 0.5f, 0.5f));
   //addColorCube(transform, cabineColor, eGameObjects::AIRPLANE_CABINE);

   /// Body
   const glm::vec4 bodyColor(0.0f, 1.0f, 1.0f, 1.0f);
   transform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
   /// Cube 0
   //transform.setTranslate(glm::vec3(0.0f, 29.0f, 0.0f));
   transform.setTranslate(glm::vec3(0.0f, -1.0f, 0.0f));
   //addColorCube(transform, bodyColor, eGameObjects::AIRPLANE);
}
