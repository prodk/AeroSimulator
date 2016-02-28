#ifndef AERO_SIMULATOR_CCAMERA_COMPONENT_H
#define AERO_SIMULATOR_CCAMERA_COMPONENT_H

#include "CComponent.h"
#include "CEventHandler.h"
#include "CTransform.h"
#include "CCamera.h"

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

      glm::mat4 getViewMatrix() const;

      glm::mat4 getProjectionMatrix() const { return mProjectionMatrix; }
      void setProjectionMatrix(const SFrustum& frustum);

   private:
      void update();
      void rotate(const unsigned int axisId, const float deltaTime);
      ///@todo: add translate look at point
      void zoom(const float deltaTime);
      void moveLeftRight(const float deltaTime);
      void moveUpDown(const float deltaTime);

      void moveInViewSpace(const bool state, const bool sign, const glm::vec3& dir, const float deltaTime);

   private:
      static const unsigned int mId = CComponent::CAMERA;

      enum eStateChanges
      {
         eChangePitch = 0, // rotate around the right vector of the camera
         eRotateY, // = 1, rotate around the up vector of the camera
         eRotateZ, // = 2, rotate around the direction of the camera
         eZoom,
         eMoveLeftRight,
         eMoveUpDown,
         eLastState
      };

      CTransform mTransform;
      std::bitset<eLastState> mStateChanges;
      std::bitset<eLastState> mStateSigns;
      glm::mat4 mProjectionMatrix;
   };
} //namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CCAMERA_COMPONENT_H
