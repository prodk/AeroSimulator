
#include "CRenderable.h"
#include "CShader.h"
#include "CGeometry.h"

using namespace AeroSimulatorEngine;

CRenderable::CRenderable()
   : mGeometry(nullptr)
   , mShader(nullptr)
{
}

CRenderable::~CRenderable()
{
}
