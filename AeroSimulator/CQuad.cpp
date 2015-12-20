#include "CQuad.h"
#include "CTexture.h"
#include "CGeometry.h"
#include "CLog.h"
#include "CRenderable.h"
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
   mTexture.reset(new CTexture());
   mGeometry.reset(new CGeometry());

   assert(mTexture);
   assert(mGeometry);

   if (mGeometry)
   {
      mGeometry->setVertexBuffer(vertices);
      const int numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
      mGeometry->setNumOfVertices(numOfVertices);

      mGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mGeometry->setNumOfIndices(numOfIndices);

      mGeometry->setNumOfElementsPerVertex(2); ///@todo: probably remove this
      mGeometry->setVertexStride(4); // 2 coords + 2 tex coords
   }
}

CQuad::~CQuad()
{
}

bool CQuad::loadTexture(const char * fileName)
{
   const bool result = (0 != mTexture->loadDDSTexture(fileName));

   if (result && (mTexture->getWidth() != mTexture->getHeight()))
   {
      glGenerateTextureMipmap(mTexture->getId());
      CLog::getInstance().log("Quad::loadTexture(): generating mipmaps for non-square texture, height: ",
         mTexture->getHeight());
   }

   return result;
}

void CQuad::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   //CLog::getInstance().log("\n** CQuad::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}
