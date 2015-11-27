#include "CBillBoard.h"
#include "CGeometry.h"
#include "CTexture.h"

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat vertices[] =
   {
      0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f, -0.5f,
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.5f,
      0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, -0.5f
   };

   GLuint indices[] =
   {
      0, 1, 2, 3
   };
}

CBillBoard::CBillBoard()
{
   mTexture.reset(new CTexture());
   mGeometry.reset(new CGeometry());

   if (mGeometry)
   {
      mGeometry->setVertexBuffer(vertices);
      const int numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
      mGeometry->setNumOfVertices(numOfVertices);

      mGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mGeometry->setNumOfIndices(numOfIndices);

      mGeometry->setNumOfElementsPerVertex(3); ///@todo: probably remove this
      mGeometry->setVertexStride(7); // 3 coords + 2 tex coords + 2 squad
   }
}

CBillBoard::~CBillBoard()
{
}

void CBillBoard::setEnvironment()
{
   glDepthMask(GL_FALSE);
}

void CBillBoard::resetEnvironment()
{
   glDepthMask(GL_TRUE);
}

bool CBillBoard::loadTexture(const char * fileName)
{
   return (0 != mTexture->loadDDSTexture(fileName));
}
