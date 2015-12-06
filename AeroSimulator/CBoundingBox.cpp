#include "CBoundingBox.h"
#include "CGeometry.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

namespace
{
   //GLfloat colorCubeData[] = {
   //   // left face
   //   // x      y      z
   //   -0.5f,  0.5f, -0.5f,
   //   -0.5f, -0.5f, -0.5f,
   //   -0.5f,  0.5f,  0.5f,
   //   -0.5f, -0.5f,  0.5f,

   //   // Right face
   //   0.5f,  0.5f,  0.5f,
   //   0.5f, -0.5f,  0.5f,
   //   0.5f,  0.5f, -0.5f,
   //   0.5f, -0.5f, -0.5f,

   //   // Front face
   //   -0.5f,  0.5f,  0.5f,
   //   -0.5f, -0.5f,  0.5f,
   //   0.5f,  0.5f,  0.5f,
   //   0.5f, -0.5f,  0.5f,

   //   // Back face
   //   0.5f,  0.5f, -0.5f,
   //   0.5f, -0.5f, -0.5f,
   //   -0.5f,  0.5f, -0.5f,
   //   -0.5f, -0.5f, -0.5f,

   //   // Top face
   //   -0.5f,  0.5f, -0.5f,
   //   -0.5f,  0.5f,  0.5f,
   //   0.5f,  0.5f, -0.5f,
   //   0.5f,  0.5f,  0.5f,

   //   // Bottom face
   //   -0.5f, -0.5f,  0.5f,
   //   -0.5f, -0.5f, -0.5f,
   //   0.5f, -0.5f,  0.5f,
   //   0.5f, -0.5f, -0.5f
   //};

   //GLuint indices[] = {
   //   0, 1, 2, 3, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9, 10, 11, 11, 12, 12, 13, 14, 15, 15, 16, 16, 17, 18, 19, 19, 20, 20, 21, 22, 23
   //};
   GLfloat colorCubeData[] = {
      //front
      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,

      // back
      -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
   };

   GLuint indices[] = {
      // Stripe 1
      6, 5, 7, 4, // back
      3, 0, // left
      2, 1, // front
      6, 5, // right

      // Stripe 2
      // bottom
      5, 1, 4, 0,

      // front
      0, 1, 2,
      2, 0, 3,

      // top
      3, 2, 6,
      6, 3, 7,

      // missing lines
      4, 5, 6, 7,
      6, 2, 1
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

bool CBoundingBox::collidesWith(const CBoundingBox & box, bool vertical) const
{
   bool result = false;

   const glm::mat4x4 boxModel = box.getModelMatrix();
   const glm::vec4 translate = boxModel[3];

   const glm::vec4 myTranslate = mModelMatrix[3];

   ///@todo: temporary, think that collision happenned if vertical distance is <=1
   float deltaX2 = 0.0f;
   if (!vertical)
      deltaX2 = (translate.x - myTranslate.x)*(translate.x - myTranslate.x);
   const float deltaY2 = (translate.y - myTranslate.y)*(translate.y - myTranslate.y);
   if (std::sqrt(deltaX2 + deltaY2) <= 1.5f)
   {
      result = true;
   }

   return result;
}
