#include "CPropeller.h"
#include "CCommonMath.h"

#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>

using namespace AeroSimulatorEngine;

const float CPropeller::mInitialSpeed = 300.0f;

CPropeller::CPropeller()
   : mRotationSpeed(mInitialSpeed)
{
}

CPropeller::~CPropeller()
{
}

void CPropeller::updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt)
{
   ///@todo: for animations add timers later
   // Rotate the propeller.
   const float deltaZ = mRotationSpeed*dt;
   calculateTRMatrix();
   mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;

   mRotate.z += deltaZ;

   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix;
      mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;
   }

   for (auto * pChild : mChildren)
   {
      if (pChild)
      {
         pChild->updateTRMatrix(mParentByLocalTRMatrix, dt); /// Avoid recalculation on every frame
      }
   }
}

void CPropeller::increaseSpeed()
{
   mRotationSpeed = std::min<float>(2000.0f, mRotationSpeed + 25.0f);
}

void CPropeller::decreaseSpeed()
{
   mRotationSpeed = std::max<float>(mInitialSpeed, mRotationSpeed - 16.0f);
}
