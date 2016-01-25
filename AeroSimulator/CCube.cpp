#include "CCube.h"
#include "CGeometry.h"
#include "../src/shaders/CShader.h"
#include "CLog.h"
#include "CColorBillBoard.h"
#include "CTexture.h"

#include <cassert>

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

///@todo: reconsider these values and their usage
const int CCube::mNumOfElementsPerVertex = 3; // 3 coordinates/color components per vertex
const int CCube::mStride = 6;                 // stride of 6 for 3 coordinates and 3 colors

CCube::CCube()
   //: CParentGameObject()
   : CGameObject()
   //, mScaledTRMatrix()
   //, mHealthBar()
   //, mHealthBarShift(glm::vec3(0.0f, 0.7f, 0.0f))
   , mHealthBarWidth(0.5f)
   , mHealthbarHeight(0.2f)
{
   //mHealthValue = 0.22f;
   ///@todo: log this only to the log file, not to stdout
   //CLog::getInstance().log("* CCube::CCube() default: success.");
}

CCube::~CCube()
{
   /*mGeometry.reset();
   mShader.reset();
   mHealthBar.reset();*/
}

//CCube::CCube(const glm::vec3 & scale,
//             const glm::vec3 & rotate,
//             const glm::vec3 & translate)
//   : CParentGameObject(scale, rotate, translate)
//{
//   CLog::getInstance().log("* CCube::CCube() non-default: success.");
//}
//
//void CCube::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
//{
//   ///@todo: think about the necessity of this log, probably remove it
//   //CLog::getInstance().log("\n** CCube::setupShadersAndBuffers() **");
//   //CGameObject::setShadersAndBuffers(pShader);
//}
//
//void CCube::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
//{
//   CParentGameObject::buildModelMatrix(parentTRMatrix);
//
//   // For the cube we need to calculate the model matrix
//   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
//   mModelMatrix = mParentTRMatrix * mScaledTRMatrix;
//}
//
//void CCube::updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt)
//{
//   CParentGameObject::updateTRMatrix(trMatrix, dt);
//
//   // Don't forget to change the cached scaled TR matrix
//   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
//}
//
//void CCube::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
//{
//   CParentGameObject::updateModelMatrix(rootModelMatrix);
//
//   // Model matrix of the cube
//   mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;
//}

//void CCube::setupHealthBar(std::shared_ptr<CShader>& pShader)
//{
//   if (pShader)
//   {
//      pShader->link();
//      //mHealthBar.reset(new CColorBillBoard());
//
//      //if (mHealthBar)
//      //{
//      //   mHealthBar->setTranslate(mHealthBarShift);
//      //   mHealthBar->setBillboardHeight(mHealthbarHeight);
//      //   mHealthBar->setBillboardWidth(mHealthBarWidth);
//      //   mHealthBar->calculateModelMatrix();
//      //   mHealthBar->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
//      //   mHealthBar->setHealthValue(mHealthValue);
//      //   mHealthBar->setTransparent(true); // This should be rendered last with the depth buffer switched off
//
//      //   mHealthBar->setShadersAndBuffers(pShader);
//      //   add(mHealthBar.get());
//      //}
//   }
//}

//bool CCube::loadTexture(const char * fileName)
//{
//   return (0 != mTexture->loadDDSTexture(fileName));
//}

//void CCube::translateHealthBar(const glm::vec3 & shift)
//{
//   /*mHealthBarShift = shift;
//   if (mHealthBar)
//      mHealthBar->setTranslate(mHealthBarShift);*/
//}

void CCube::setHealth(float health)
{
  /* mHealthValue = std::min<float>(1.0, std::max<float>(0.0, health));
   if (mHealthBar)
      mHealthBar->setHealthValue(mHealthValue);*/
}
