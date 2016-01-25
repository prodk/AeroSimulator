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

   /*if (mTimeSinceLastFrame >= animationTime)
   {
      mTimeSinceLastFrame = 0.0f;

      ++mCurrentFrame.x;
      if (mCurrentFrame.x >= mNumOfFrames.x)
      {
         mCurrentFrame.x = 0;
         ++mCurrentFrame.y;
         mCurrentFrame.y = static_cast<int>(mCurrentFrame.y) % static_cast<int>(mNumOfFrames.y);
      }
   }*/

   mTimeSinceLastFrame += deltaTime;
}

//void CAnimationBillBoard::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
//{
   //CParentGameObject::updateModelMatrix(rootModelMatrix);

   //mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;

   //// Reset parent-related rotations - billboard rotates ONLY relatively to the camera direction
   //mModelMatrix[0].x = 1.0f;
   //mModelMatrix[0].y = 0.0f;
   //mModelMatrix[0].z = 0.0f;

   //mModelMatrix[1].x = 0.0f;
   //mModelMatrix[1].y = 1.0f;
   //mModelMatrix[1].z = 0.0f;

   //mModelMatrix[2].x = 0.0f;
   //mModelMatrix[2].y = 0.0f;
   //mModelMatrix[2].z = 1.0f;
//}
