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

      // CEventHandler part
      virtual void handleEvent(CAppEvent *pEvent) override;

      CTransform& getTransform() { return mTransform; }
      void setTransform(const CTransform& transform) { mTransform = transform; }

   private:
      void updatePitch(const float deltaTime);
      void rotateAroundAxisOfSymmetry(const float deltaTime);

   private:
      static const unsigned int mId = CComponent::CAMERA;

      enum eStateChanges
      {
         eIncreasePitch = 1,
         eDecreasePitch, //2
         eRotateCw, //3
         eRotateCcw //4
      };

      CTransform mTransform;
      unsigned int mStateChanges;
   };
} //namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CCAMERA_COMPONENT_H
