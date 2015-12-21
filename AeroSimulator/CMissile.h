#ifndef AERO_SIMULATOR_CMISSILE_H
#define AERO_SIMULATOR_CMISSILE_H

#include "CCube.h"

namespace AeroSimulatorEngine
{
   class CMissile : public CCube
   {
   public:
      CMissile();
      virtual ~CMissile();

      bool isDetached() const { return mIsDetached; }
      void setDetached(const bool detach) { mIsDetached = detach; }

      void setFlightDirection(const glm::vec3& dir) { mFlightDirection = dir; }
      void update(float dt);

   private:
      bool mIsDetached;
      glm::vec3 mFlightDirection;
      glm::vec3 mSpeed;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CMISSILE_H
