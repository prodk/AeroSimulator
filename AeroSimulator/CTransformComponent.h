#ifndef AERO_SIMULATOR_CTRANSFORM_COMPONENT_H
#define AERO_SIMULATOR_CTRANSFORM_COMPONENT_H

#include "CComponent.h"
#include "CTransform.h"
#include "CEventHandler.h"

namespace AeroSimulatorEngine
{
   class CGameObject;

   ///@todo: add processing update event here so that 
   ///@todo: if no Movement component exist the model matrix is still added to the renderabl
   class CTransformComponent : public CComponent, public CEventHandler
   {
   public:
      static unsigned int getId() { return mId; }

      explicit CTransformComponent(CGameObject* pOwner);
      virtual ~CTransformComponent();

      CTransform& getTransform() { return mTransform; }
      void setTransform(const CTransform& transform) { mTransform = transform; }

      // CEventHandler part
      virtual void handleEvent(CAppEvent *pEvent) override;

   private:
      void update();

   private:
      /// Id of the component
      static const unsigned int mId = CComponent::TRAHSFORM;

      CTransform mTransform;
   };
} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CTRANSFORM_COMPONENT_H
