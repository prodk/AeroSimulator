#include "CCube.h"
#include "CGeometry.h"
#include "CShader.h"
#include "CLog.h"

#include <cassert>

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

///@todo: reconsider these values and their usage
const int CCube::mNumOfElementsPerVertex = 3; // 3 coordinates/color components per vertex
const int CCube::mStride = 6;                 // stride of 6 for 3 coordinates and 3 colors

CCube::CCube()
   : CParentGameObject()
   , mScaledTRMatrix()
{
   CLog::getInstance().log("* CCube::CCube() default: success.");
}

CCube::~CCube()
{
   mGeometry.reset();
   mShader.reset();
}

CCube::CCube(const glm::vec3 & scale,
             const glm::vec3 & rotate,
             const glm::vec3 & translate)
   : CParentGameObject(scale, rotate, translate)
{
   CLog::getInstance().log("* CCube::CCube() non-default: success.");
}

void CCube::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().log("\n** CCube::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

void CCube::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   CParentGameObject::buildModelMatrix(parentTRMatrix);

   // For the cube we need to calculate the model matrix
   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
   mModelMatrix = mParentTRMatrix * mScaledTRMatrix;
}

void CCube::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   CParentGameObject::updateTRMatrix(trMatrix);

   // Don't forget to change the cached scaled TR matrix
   if (trMatrix != mParentTRMatrix)
   {
      mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
   }
}

void CCube::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   CParentGameObject::updateModelMatrix(rootModelMatrix);

   // Model matrix of the cube
   mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;
}
