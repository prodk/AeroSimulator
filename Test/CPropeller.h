#ifndef AERO_SIMULATOR_CPROPELLER_H
#define AERO_SIMULATOR_CPROPELLER_H

#include "../AeroSimulator/CGameObject.h"

namespace AeroSimulatorEngine
{
   class CTransform;
   class CShader;

   class CPropeller : public CGameObject
   {
   public:
      CPropeller(const int id, const int type, std::shared_ptr<CShader> shader, float speed);
      virtual ~CPropeller();

      void move() override;
      void setRotationSpeed(const float speed) { mRotSpeed = speed; }

   private:
      std::shared_ptr<CShader> mShader;
      float mRotSpeed;
   };
} // AeroSimulatorEngine
#endif // AERO_SIMULATOR_CPROPELLER_H
