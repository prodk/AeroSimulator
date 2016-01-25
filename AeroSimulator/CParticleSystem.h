#ifndef AERO_SIMULATOR_CPARTICLE_SYSTEM_H
#define AERO_SIMULATOR_CPARTICLE_SYSTEM_H

//#include "CParentGameObject.h"
#include "CGameObject.h"
#include <set>
#include "../AeroSimulator/include/glm/vec3.hpp"
#include "../AeroSimulator/include/glm/mat4x4.hpp"

namespace AeroSimulatorEngine
{
   class CAnimationBillBoard;
   class CShader;

   struct SParticle
   {
      SParticle::SParticle();

      glm::vec3 mPosition;
      glm::vec3 mVelocity;
      float mCurrentTime;
      bool mIsAlive;
   };

   //class CParticleSystem : public CParentGameObject
   class CParticleSystem : public CGameObject
   {
   public:
      CParticleSystem(float maxLifeTime, float emitSpeed, int numOfParticles, const glm::vec3 & particleSpeed);
      virtual ~CParticleSystem();

      bool emit(float deltaTime);
      virtual void update(const float deltaTime);
      void setEmitSpeed(const float factor);
      void resetEmitSpeed() { mTimeToEmit = 1.0f / mEmitSpeed; }
      /*void addParticles(std::shared_ptr<CShader>& pShader, std::shared_ptr<CShader>& pColorShader, 
                        const char* filePath, const glm::vec2 & frameSize, const float width, const float height);*/

      /// Composite-related methods - override only some of them
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));

      void showParticles(const bool factor);

   private:
      std::vector<SParticle> mParticle; ///@todo: change to a collection of particles
      std::vector<std::unique_ptr<CAnimationBillBoard> > mBillboard; ///@todo: probably make a member of SParticle

      float mSystemTime;
      float mMaxLifeTime; // In seconds ///@todo: make a constant
      glm::mat4x4 mScaledTRMatrix;
      float mEmitSpeed; // Particles per second
      float mTimeToEmit;
      std::size_t mNumOfParticles;
      int mCurrentParticle;
      //std::set<int> mAliveParticles;
      glm::vec3 mParticleSpeed;
      std::string mTexturePath;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CPARTICLE_SYSTEM_H
