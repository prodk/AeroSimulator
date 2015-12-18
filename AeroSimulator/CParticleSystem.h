#ifndef AERO_SIMULATOR_CPARTICLE_SYSTEM_H
#define AERO_SIMULATOR_CPARTICLE_SYSTEM_H

#include "CLeafGameObject.h"

namespace AeroSimulatorEngine
{
   class CAnimationBillBoard;

   struct SParticle
   {
      SParticle::SParticle();

      glm::vec3 mPosition;
      glm::vec3 mVelocity;
      float mCurrentTime;
   };

   class CParticleSystem :
      public CLeafGameObject
   {
   public:
      CParticleSystem();
      virtual ~CParticleSystem();

      bool emit(float deltaTime);
      virtual void update(const float deltaTime);

   private:
      SParticle mParticle; ///@todo: change to a collection of particles
      std::unique_ptr<CAnimationBillBoard> mBillboard;
      float mCurrentTime;
      float mMaxLifeTime; // In seconds ///@todo: make a constant
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CPARTICLE_SYSTEM_H
