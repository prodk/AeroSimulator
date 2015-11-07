#include "CGameObject.h"

using namespace AeroSimulatorEngine;

CGameObject::CGameObject()
   : CRenderable()
   , mModelMatrix()
   , mScale()
   , mRotate()
   , mTranslate()
{
}

CGameObject::~CGameObject()
{
}

CGameObject::CGameObject(const glm::mat4 & parentModelMatrix,
                          const glm::vec3 & scale,
                          const glm::vec3 & rotate,
                          const glm::vec3 & translate)
   : CRenderable()
   , mModelMatrix(parentModelMatrix)
   , mScale(scale)
   , mRotate(rotate)
   , mTranslate(translate)
{
}
