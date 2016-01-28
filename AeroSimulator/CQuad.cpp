#include "CQuad.h"
#include "CTexture.h"
#include "CGeometry.h"
#include "CLog.h"
#include "CRenderable.h"
#include "CRenderableComponent.h"
#include "../src/shaders/CShader.h"

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat vertices[] =
   {
      // Positions   // TexCoords
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 0.0f,
       1.0f, -1.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  1.0f, 1.0f
   };

   GLuint indices[] =
   {
      0, 1, 3, 2
   };
}

CQuad::CQuad()
{
   // A quad has only a renderable component
   addComponent<CRenderableComponent>();
}

CQuad::~CQuad()
{
}

void CQuad::prepareRenderable(std::shared_ptr<CShader>& pShader)
{
   getRenderable().setGeometry(vertices, indices);
   getRenderable().createTexture(MAIN_TEXTURE);

   if (pShader)
   {
      getRenderable().setShader(pShader);
   }
}

void CQuad::setShader(std::shared_ptr<CShader>& pShader)
{
   getRenderable().setShader(pShader);
}

void CQuad::setTextureUnit(const GLint unit)
{
   getRenderable().set1DParam(TEXTURE_UNIT, unit);
}

CRenderable & CQuad::getRenderable()
{
   CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(*this);
   if (pRenderableComp)
   {
      return pRenderableComp->getRenderable();
   }
}
