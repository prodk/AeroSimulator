#ifndef AERO_SIMULATOR_CPARTICLE_SYSTEM_H
#define AERO_SIMULATOR_CPARTICLE_SYSTEM_H

#include "CParentGameObject.h"

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

   class CParticleSystem : public CParentGameObject
   {
   public:
      CParticleSystem();
      virtual ~CParticleSystem();

      bool emit(float deltaTime);
      virtual void update(const float deltaTime);
      void addParticle(std::shared_ptr<CShader>& pShader, std::shared_ptr<CShader>& pColorShader);

      /// Composite-related methods - override only some of them
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));

   private:
      SParticle mParticle; ///@todo: change to a collection of particles
      std::unique_ptr<CAnimationBillBoard> mBillboard;
      float mCurrentTime;
      float mMaxLifeTime; // In seconds ///@todo: make a constant
      glm::mat4x4 mScaledTRMatrix;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CPARTICLE_SYSTEM_H
