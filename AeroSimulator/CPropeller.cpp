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

void CPropeller::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   ///@todo: for animations add timers later
   // Rotate the propeller.
   const float deltaZ = 4.0f;
   calculateTRMatrix();
   mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;

   mRotate.z += deltaZ;

   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix;
      ///@todo: update the cached product of the parentTR by the current localTR here;
      mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;
   }

   for (auto * pChild : mChildren)
   {
      if (pChild)
      {
         pChild->updateTRMatrix(mParentByLocalTRMatrix); ///avoid recalculation on every frame
      }
   }
}
