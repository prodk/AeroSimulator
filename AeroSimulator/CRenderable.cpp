
#include "CRenderable.h"
#include "CShader.h"
#include "CGeometry.h"

using namespace AeroSimulatorEngine;

CRenderable::CRenderable()
   : mGeometry()
   , mShader()
   , mModelMatrix()
   , mVboId(0)
   , mIboId(0)
   , mParentModelMatrix()
   , mMvpMatrix()
   , mTexture()
{
}

CRenderable::~CRenderable()
{
   mGeometry.reset();
   mShader.reset();
}

bool CRenderable::loadTexture(const char * filePath)
{
   return false;
}
