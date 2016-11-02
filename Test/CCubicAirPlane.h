#ifndef AERO_SIMULATOR_CUBIC_AERO_PLANE_H
#define AERO_SIMULATOR_CUBIC_AERO_PLANE_H

#include "../AeroSimulator/CGameObject.h"
#include <map>
#include <bitset>

namespace AeroSimulatorEngine
{
   class CTransform;
   class CPropeller;

   class CCubicAirPlane : public CGameObject
   {
   public:
      CCubicAirPlane(const int id, const int type, std::shared_ptr<CShader> shader);
      virtual ~CCubicAirPlane();

      void move() override;
      void specificMove(const int moveType) override;

   private:
      void lean(CTransform & rt);
      void changeHeight(CTransform & rt);
      void createParts();

   private:
      ///@todo: move the airplane manipulation code to a separate base class and inherit from it
      enum eCurrentAction { LEAN_ACT, GO_ACT, LAST_ACTION };
      enum eCurrentState { LEAN_LEFT, LEAN_RIGHT, GO_UP, GO_DOWN, LAST_STATE };

      std::shared_ptr<CShader> mShader;
      float mRollAngle;
      float mPitch;
      std::bitset<eCurrentAction::LAST_ACTION> mCurrentAction;
      std::bitset<eCurrentState::LAST_STATE> mCurrentState;
      std::shared_ptr<CPropeller> mPropeller;
      const float mMinPropellerSpeed = 4.0f;
      const float mMaxPropellerSpeed = 24.0f;
      float mPropellerSpeed;
   };
} // AeroSimulatorEngine
#endif // AERO_SIMULATOR_CUBIC_AERO_PLANE_H
