
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
{
}

CRenderable::~CRenderable()
{
   mGeometry.reset();
   mShader.reset();
}
