#include "CBoundingBox.h"
#include "CGeometry.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat colorCubeData[] = {
      // left face
      // x      y      z
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,

      // Right face
      0.5f,  0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,

      // Front face
      -0.5f,  0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,

      // Back face
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,

      // Top face
      -0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f,  0.5f,

      // Bottom face
      -0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f, -0.5f
   };

   GLuint indices[] = {
      0, 1, 2, 3, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9, 10, 11, 11, 12, 12, 13, 14, 15, 15, 16, 16, 17, 18, 19, 19, 20, 20, 21, 22, 23
   };
}

CBoundingBox::CBoundingBox()
{
   setDrawWithLines(true);

   mGeometry.reset(new CGeometry());

   if (mGeometry)
   {
      mGeometry->setVertexBuffer(colorCubeData);
      const int numOfVertices = sizeof(colorCubeData) / sizeof(colorCubeData[0]);
      mGeometry->setNumOfVertices(numOfVertices);

      mGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mGeometry->setNumOfIndices(numOfIndices);

      mGeometry->setNumOfElementsPerVertex(CCube::mNumOfElementsPerVertex);
      mGeometry->setVertexStride(3); // 3 coords and nothing else
      CLog::getInstance().log("CBoundingBox geometry has been setup.");
   }
}

CBoundingBox::~CBoundingBox()
{
}
