#include "CCube.h"
#include "CGeometry.h"
#include "CShader.h"
#include "CLog.h"
//#include "CBillBoard.h"
#include "CColorBillBoard.h"
#include "CTexture.h"

#include <cassert>

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

///@todo: reconsider these values and their usage
const int CCube::mNumOfElementsPerVertex = 3; // 3 coordinates/color components per vertex
const int CCube::mStride = 6;                 // stride of 6 for 3 coordinates and 3 colors

CCube::CCube()
   : CParentGameObject()
   , mScaledTRMatrix()
   , mHealthBarBack()
   , mHealthBarFore()
   , mHealthBarShift(glm::vec3(0.0f, 0.7f, 0.0f))
{
   CLog::getInstance().log("* CCube::CCube() default: success.");
}

CCube::~CCube()
{
   mGeometry.reset();
   mShader.reset();
   mHealthBarBack.reset();
   mHealthBarFore.reset();
}

CCube::CCube(const glm::vec3 & scale,
             const glm::vec3 & rotate,
             const glm::vec3 & translate)
   : CParentGameObject(scale, rotate, translate)
   , mScaledTRMatrix()
   , mHealthBarBack()
   , mHealthBarFore()
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

void CCube::setupHealthBar(std::shared_ptr<CShader>& pShader)
{
   if (pShader)
   {
      pShader->link();
      mHealthBarBack.reset(new CColorBillBoard());
      mHealthBarFore.reset(new CColorBillBoard());

      if (mHealthBarBack && mHealthBarFore)
      {
         const float bgWidth = 0.6f;
         // Background billboard
         mHealthBarBack->setTranslate(mHealthBarShift);
         mHealthBarBack->setBillboardHeight(0.3f);
         mHealthBarBack->setBillboardWidth(bgWidth);
         mHealthBarBack->calculateModelMatrix();
         mHealthBarBack->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

         mHealthBarBack->setShadersAndBuffers(pShader);
         add(mHealthBarBack.get());

         const float fgWidth = 0.4f;
         // Foreground billboard
         glm::vec3 fgShift = mHealthBarShift;
         fgShift.x -= 0.5f*(bgWidth - fgWidth);
         //mHealthBarFore->setTranslate(fgShift);

         mHealthBarFore->setTranslate(mHealthBarShift);
         mHealthBarFore->setBillboardHeight(0.3f);
         mHealthBarFore->setBillboardWidth(0.4f);
         mHealthBarFore->calculateModelMatrix();
         mHealthBarFore->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

         mHealthBarFore->setShadersAndBuffers(pShader);
         add(mHealthBarFore.get());
      }
   }
}

bool CCube::loadTexture(const char * fileName)
{
   return (0 != mTexture->loadDDSTexture(fileName));
}

void CCube::translateHealthBar(const glm::vec3 & shift)
{
   mHealthBarShift = shift;
   mHealthBarBack->setTranslate(mHealthBarShift);
   mHealthBarFore->setTranslate(mHealthBarShift);
}
