#ifndef AERO_SIMULATOR_CMISSILE_H
#define AERO_SIMULATOR_CMISSILE_H

#include "CCube.h"

namespace AeroSimulatorEngine
{
   class CParticleSystem;
   class CWin32Renderer;

   class CMissile : public CCube
   {
   public:
      CMissile();
      virtual ~CMissile();

      bool isDetached() const { return mIsDetached; }
      void setDetached(const bool detach) { mIsDetached = detach; }

      void setFlightDirection(const glm::vec3& dir) { mFlightDirection = dir; }
      void update(float dt);

      void addParticles(std::shared_ptr<CShader>& pShader, std::shared_ptr<CShader>& pColorShader,
                        const char* filePath, const glm::vec2 & frameSize, const float width, const float height);

      void setEmitSpeed(const float factor);
      void resetEmitSpeed();
      void setFireVisible(const bool visible);

   private:
      bool mIsDetached;
      glm::vec3 mFlightDirection;
      glm::vec3 mSpeed;
      std::shared_ptr<CParticleSystem> mFire;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CMISSILE_H
