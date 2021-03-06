#ifndef AERO_SIMULATOR_MOVEMENT_COMPONENT_H
#define AERO_SIMULATOR_MOVEMENT_COMPONENT_H

#include "CComponent.h"
#include "CEventHandler.h"
#include "../AeroSimulator/include/glm/vec3.hpp"

namespace AeroSimulatorEngine
{
   class CGameObject;

   class CMovementComponent : public CComponent, public CEventHandler
   {
   public:
      static unsigned int getId() { return mId; }

      explicit CMovementComponent(CGameObject* pOwner);
      virtual ~CMovementComponent();

      // CEventHandler part
      virtual void handleEvent(CAppEvent *pEvent) override;

   private:
      static const unsigned int mId = CComponent::MOVEMENT;
   };
} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_MOVEMENT_COMPONENT_H
