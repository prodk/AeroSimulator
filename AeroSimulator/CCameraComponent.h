#ifndef AERO_SIMULATOR_CCAMERA_COMPONENT_H
#define AERO_SIMULATOR_CCAMERA_COMPONENT_H

#include "CComponent.h"
#include "CEventHandler.h"
#include "CTransform.h"

#include <bitset>

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
      void rotate(const unsigned int axisId, const float deltaTime);
      ///@todo: add translate look at point
      void zoom(const float deltaTime);

   private:
      static const unsigned int mId = CComponent::CAMERA;

      enum eStateChanges
      {
         eChangePitch = 0,
         eRotateY, // = 1,
         eRotateZ, // = 2,
         eZoom, 
         eLastState
      };

      CTransform mTransform;
      std::bitset<eLastState> mStateChanges;
      std::bitset<eLastState> mStateSigns;
   };
} //namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CCAMERA_COMPONENT_H
