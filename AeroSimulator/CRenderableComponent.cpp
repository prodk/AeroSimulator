#include "CRenderableComponent.h"
#include "CRenderable.h"
#include "src\shaders\CShader.h"

#include <cassert>

using namespace AeroSimulatorEngine;

//CRenderableComponent::CRenderableComponent(CGameObject * pOwner)
//   : CComponent(pOwner)
//   , mRenderable(new AeroSimulatorEngine::CRenderable())
//{
//   assert(mRenderable);
//}
CRenderableComponent::CRenderableComponent(CGameObject* pOwner, GLfloat* pVertices, GLuint* pIndices,
                                           std::shared_ptr<CShader>& pShader, const char* mainTextureFilePath)
   : CComponent(pOwner)
   , mRenderable(new CRenderable(pVertices, pIndices, pShader, mainTextureFilePath))
{
   assert(mRenderable);
}

CRenderableComponent::~CRenderableComponent()
{
}

void CRenderableComponent::init()
{
}
