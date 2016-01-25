#ifndef AERO_SIMULATOR_CPROPELLER_OBJECT_H
#define AERO_SIMULATOR_CPROPELLER_OBJECT_H

#include "CParentGameObject.h"

namespace AeroSimulatorEngine
{
   class CPropeller : public CParentGameObject
   {
   public:
      CPropeller();
      virtual ~CPropeller();

      //virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt);

      void increaseSpeed();
      void decreaseSpeed();
      void setSpeed(const float speed) { mRotationSpeed = speed; }

   private:
      float mRotationSpeed;
      static const float mInitialSpeed;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CPROPELLER_OBJECT_H
