#ifndef AERO_SIMULATOR_CTRANSFORM_COMPONENT_H
#define AERO_SIMULATOR_CTRANSFORM_COMPONENT_H

#include "CComponent.h"
#include "CTransform.h"

namespace AeroSimulatorEngine
{
   class CGameObject;

   class CTransformComponent : public CComponent
   {
   public:
      static unsigned int getId() { return mId; }

      explicit CTransformComponent(CGameObject* pOwner);
      virtual ~CTransformComponent();

      CTransform& getTransform() { return mTransform; }
      void setTransform(const CTransform& transform) { mTransform = transform; }

   private:
      /// Id of the component
      static const unsigned int mId = CComponent::TRAHSFORM;

      CTransform mTransform;
   };
} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CTRANSFORM_COMPONENT_H
