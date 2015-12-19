#ifndef AERO_SIMULATOR_CANIMATION_BILLBOARD_H
#define AERO_SIMULATOR_CANIMATION_BILLBOARD_H

#include "CBillBoard.h"

namespace AeroSimulatorEngine
{
   class CAnimationBillBoard : public CBillBoard
   {
   public:
      CAnimationBillBoard();
      virtual ~CAnimationBillBoard();

      virtual void update(const float deltaTime);

      void setAnimationSpeed(const float speed) { mAnimationSpeed = speed; }
      void resetCurrentFrame() { mCurrentFrame = glm::vec2(); }

      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));

   private:
      float mTimeSinceLastFrame;
      float mAnimationSpeed;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CANIMATION_BILLBOARD_H
