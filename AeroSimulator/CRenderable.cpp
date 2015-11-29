
#include "CRenderable.h"
#include "../src/shaders/CShader.h"
#include "CGeometry.h"

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
   , mHealthbarShift(0.0f)
   , mViewMatrix()
   , mEyePos()
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
