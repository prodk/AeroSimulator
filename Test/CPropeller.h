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
      CPropeller(const int id, const int type, std::shared_ptr<CShader> shader);
      virtual ~CPropeller();

      void move() override;

   private:
      void addColorCube(const CTransform & transform, const glm::vec4 & color, const int objectType);///@todo: remove this copypasted code CCubicAirPlane

   private:
      std::shared_ptr<CShader> mShader;
   };
} // AeroSimulatorEngine
#endif // AERO_SIMULATOR_CPROPELLER_H
