#include "CTransform.h"

using namespace AeroSimulatorEngine;

CTransform::CTransform()
   : mScale(1.0f, 1.0f, 1.0f)
   , mRotate(0.0f, 0.0f, 0.0f)
   , mTranslate(0.0f, 0.0f, 0.0f)
   //, mTRMatrix()
   , mModelMatrix()
{
}

CTransform::~CTransform()
{
}

void CTransform::setScale(const glm::vec3& scale)
{
   mScale = scale;
}

void CTransform::setRotate(const glm::vec3 & rotate)
{
   mRotate = rotate;
}

void AeroSimulatorEngine::CTransform::setTranlate(const glm::vec3 & translate)
{
   mTranslate = translate;
}
