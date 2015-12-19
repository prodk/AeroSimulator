#include "CAnimationBillBoard.h"
#include "CTexture.h"

using namespace AeroSimulatorEngine;

///@todo: remove this class, it is redundant, when CGameObject contains update()
CAnimationBillBoard::CAnimationBillBoard()
   : mTimeSinceLastFrame(0.0f)
   , mAnimationSpeed(16.0f)
{
}

CAnimationBillBoard::~CAnimationBillBoard()
{
}

void CAnimationBillBoard::update(const float deltaTime)
{
   ///@todo: make members
   //const float animationSpeed = 16.0f; // frames per second
   const float animationTime = 1.0f / mAnimationSpeed;

   if (mTimeSinceLastFrame >= animationTime)
   {
      mTimeSinceLastFrame = 0.0f;

      ++mCurrentFrame.x;
      if (mCurrentFrame.x >= mNumOfFrames.x)
      {
         mCurrentFrame.x = 0;
         ++mCurrentFrame.y;
         mCurrentFrame.y = static_cast<int>(mCurrentFrame.y) % static_cast<int>(mNumOfFrames.y);
      }
   }

   mTimeSinceLastFrame += deltaTime;
}
