
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
   //, mCanBeRendered(true)
{
}

CRenderable::~CRenderable()
{
   //mGeometry.reset();
   //mShader.reset();
   //mTexture.reset();
}

bool CRenderable::loadTexture(const char * filePath)
{
   return false;
}
