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
   : mIsInitialized(false)
{
   // A quad has only a renderable component
   mIsInitialized = addComponent<CRenderableComponent>();
}

CQuad::~CQuad()
{
}

void CQuad::prepareRenderable(std::shared_ptr<CShader>& pShader, int textureId, GLuint openGlId, GLenum textureUnit)
{
   if (mIsInitialized)
   {
      LOG("* CQuad::prepareRenderable()");
      const int numVert = sizeof(vertices) / sizeof(vertices[0]);
      const int numInd = sizeof(indices) / sizeof(indices[0]);
      const int elementsPerVertex = 2;
      const int stride = 4; // 2 coords + 2 tex coords
      SGeometryData geometryData(vertices, numVert, indices, numInd, elementsPerVertex, stride);

      getRenderable().setGeometry(geometryData);

      getRenderable().createTexture(textureId);

      if (0 != openGlId)
      {
         setTextureId(textureId, openGlId);
      }

      if (GL_TEXTURE0 != textureUnit)
      {
         setTextureUnit(textureUnit);
      }

      getRenderable().setShader(pShader);
   }
}

void CQuad::setShader(std::shared_ptr<CShader>& pShader)
{
   getRenderable().setShader(pShader);
}

void CQuad::setTextureId(const int textureId, const GLuint id)
{
   getRenderable().getTexture(textureId)->setId(id);
}

void CQuad::setTextureUnit(const GLenum unit)
{
   getRenderable().set1DParam(TEXTURE_UNIT, (float)unit);
}

CRenderable & CQuad::getRenderable()
{
   CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(*this);
   assert(pRenderableComp);

   return pRenderableComp->getRenderable();
}
