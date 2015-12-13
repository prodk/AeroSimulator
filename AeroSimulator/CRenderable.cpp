
#include "CRenderable.h"
#include "../src/shaders/CShader.h"
#include "CGeometry.h"
#include "CCommonMath.h"

#include "glm/mat3x3.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CRenderable::CRenderable()
   : mGeometry()
   , mShader()
   , mModelMatrix()
   , mMvpMatrix()
   , mTexture()
   , mVboId(0)
   , mIboId(0)
   , mRightVector()
   , mUpVector()
   , mBillboardWidth(1.0f)
   , mBillboardHeight(1.0f)
   , mDrawWithLines(false)
   , mColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
   , mLineWidth(2.0f)
   , mHealthValue(0.0f)
   , mViewMatrix()
   , mEyePos()
   , mCurrentFrame()
   , mFrameSize()
   , mNumOfFrames()
   , mIsVisible(true)
   , mNormalMapTexture()
   , mAnimationTexture()
   , mRepeatTexture(false)
{
}

CRenderable::~CRenderable()
{
   mGeometry.reset();
   mShader.reset();
   mTexture.reset();
}

void CRenderable::resetEnvironment()
{
}

void CRenderable::setEnvironment()
{
}

bool CRenderable::loadTexture(const char * filePath)
{
   return false;
}

bool CRenderable::loadNormalMapTexture(const char * filePath)
{
   return false;
}

bool CRenderable::loadAnimationTexture(const char * filePath)
{
   return false;
}

glm::mat3 CRenderable::getNormalMatrix() const
{
   glm::mat3x3 result;
   CCommonMath::copyColumn(0, result, mModelMatrix);
   CCommonMath::copyColumn(1, result, mModelMatrix);
   CCommonMath::copyColumn(2, result, mModelMatrix);

   result = glm::inverse(result);
   result = glm::transpose(result);

   return result;
}
