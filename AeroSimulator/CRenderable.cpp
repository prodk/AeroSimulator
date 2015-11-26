
#include "CRenderable.h"
#include "CShader.h"
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
   , mColor()
   , mLineWidth(2.0f)
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
