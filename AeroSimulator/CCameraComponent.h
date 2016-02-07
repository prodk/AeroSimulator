#ifndef AERO_SIMULATOR_CCAMERA_COMPONENT_H
#define AERO_SIMULATOR_CCAMERA_COMPONENT_H

#include "CComponent.h"
#include "CEventHandler.h"
#include "CTransform.h"

namespace AeroSimulatorEngine
{
   class CGameObject;

   class CCameraComponent : public CComponent, public CEventHandler
   {
   public:
      explicit CCameraComponent(CGameObject* pOwner);
      virtual ~CCameraComponent();

      static unsigned int getId() { return mId; }

      virtual void init() override; ///@todo: probably remove this useless method

      // CEventHandler part
      virtual void handleEvent(CAppEvent *pEvent) override;

      CTransform& getTransform() { return mTransform; }
      void setTransform(const CTransform& transform) { mTransform = transform; }

   private:
      static const unsigned int mId = CComponent::CAMERA;

      CTransform mTransform;
   };
} //namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CCAMERA_COMPONENT_H
