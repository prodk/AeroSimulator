#include "CAnimationBillBoard.h"
#include "CTexture.h"

using namespace AeroSimulatorEngine;

///@todo: remove this class, it is redundant, when CGameObject contains update()
CAnimationBillBoard::CAnimationBillBoard()
   : mTimeSinceLastFrame(0.0f)
{
}

CAnimationBillBoard::~CAnimationBillBoard()
{
}

void CAnimationBillBoard::update(const float deltaTime)
{
   ///@todo: make members
   const float animationSpeed = 16.0f; // frames per second
   const float animationTime = 1.0f / animationSpeed;

   if (mTimeSinceLastFrame >= animationTime)
   {
      mTimeSinceLastFrame = 0.0f;

      ++mCurrentFrame;
      mCurrentFrame.x = static_cast<int>(mCurrentFrame.x) % static_cast<int>(mNumOfFrames.x);
      mCurrentFrame.y = static_cast<int>(mCurrentFrame.y) % static_cast<int>(mNumOfFrames.y);
   }

   mTimeSinceLastFrame += deltaTime;
}
