#ifndef AERO_SIMULATOR_CUBIC_AERO_PLANE_H
#define AERO_SIMULATOR_CUBIC_AERO_PLANE_H

#include "../AeroSimulator/CGameObject.h"
#include <map>

namespace AeroSimulatorEngine
{
   class CTransform;

   class CCubicAirPlane : public CGameObject
   {
   public:
      CCubicAirPlane(const int id, const int type, std::shared_ptr<CShader> shader);
      virtual ~CCubicAirPlane();

      void move() override;
      bool getChildren(std::map<int, std::shared_ptr<CGameObject>> & kids) override;

   private:
      void addCubes();
      void addColorCube(const CTransform & transform, const glm::vec4 & color, const int objectType);

   private:
      std::shared_ptr<CShader> mShader;
      std::map<int, std::shared_ptr<CGameObject>> mCubes;
   };
} // AeroSimulatorEngine
#endif // AERO_SIMULATOR_CUBIC_AERO_PLANE_H
