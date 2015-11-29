#include "CPropeller.h"
#include "CCommonMath.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CPropeller::CPropeller()
{
}

CPropeller::~CPropeller()
{
}

void CPropeller::updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt)
{
   ///@todo: for animations add timers later
   // Rotate the propeller.
   const float deltaZ = 300.0f*dt;
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
