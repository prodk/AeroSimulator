#ifndef AERO_SIMULATOR_CUBIC_AERO_PLANE_H
#define AERO_SIMULATOR_CUBIC_AERO_PLANE_H

#include "../AeroSimulator/CGameObject.h"
#include <map>

namespace AeroSimulatorEngine
{
   class CCubicAeroPlane : public CGameObject
   {
   public:
      CCubicAeroPlane(const int id, const int type);
      virtual ~CCubicAeroPlane();

      void move() override;

   private:
      void addCubes();

   private:
      std::map<int, std::shared_ptr<CGameObject>> mCubes;
   };
} // AeroSimulatorEngine
#endif // AERO_SIMULATOR_CUBIC_AERO_PLANE_H
