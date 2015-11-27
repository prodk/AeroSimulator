#include "CColorBillBoard.h"
#include "CGeometry.h"

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat vertices[] =
   {
      0.0f, 0.0f, 0.0f, -0.5f, 0.5f,
      0.0f, 0.0f, 0.0f, -0.5f, -0.5f,
      0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
      0.0f, 0.0f, 0.0f, 0.5f, -0.5f
   };

   GLuint indices[] =
   {
      0, 1, 2, 3
   };
}

CColorBillBoard::CColorBillBoard()
{
   mGeometry.reset(new CGeometry());

   if (mGeometry)
   {
      mGeometry->setVertexBuffer(vertices);
      const int numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
      mGeometry->setNumOfVertices(numOfVertices);

      mGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mGeometry->setNumOfIndices(numOfIndices);

      mGeometry->setNumOfElementsPerVertex(3);
      mGeometry->setVertexStride(5);
   }
}

CColorBillBoard::~CColorBillBoard()
{
}

void CColorBillBoard::setEnvironment()
{
   glDepthMask(GL_FALSE);
}

void CColorBillBoard::resetEnvironment()
{
   glDepthMask(GL_TRUE);
}
