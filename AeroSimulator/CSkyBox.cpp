#include "CSkyBox.h"
#include "CGeometry.h"
#include "CCube.h"

#include <cassert>

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat vertices[] =
   {
      // left face
      // x      y      z     s     t
      -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,

      // Right face
      0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
      0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

      // Front face
      -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
      0.5f, -0.5f,  0.5f, 1.0f, 0.0f,

      // Back face
      0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

      // Top face
      -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

      // Bottom face
      -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f
   };

   GLuint indices[] =
   {
      //    c     c     c     c     c     c     c       c       c       c       c       c       c       c       c       c
      0, 1, 2, 3, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9, 10, 11, 11, 12, 12, 13, 14, 15, 15, 16, 16, 17, 18, 19, 19, 20, 20, 21, 22, 23
   };

   //// Unit cube
   //GLfloat cubeData[] = {
   //   //front
   //   -0.5f, -0.5f,  0.5f,
   //   1.0f, 0.0f, 0.0f, //0 color
   //   0.5f, -0.5f,  0.5f,
   //   0.0f, 1.0f, 0.0f, //1 color
   //   0.5f,  0.5f,  0.5f,
   //   0.0f, 0.0f, 1.0f, //2 color
   //   -0.5f,  0.5f,  0.5f,
   //   0.2f, 0.0f, 0.5f, //3 color
   //                     // back
   //   -0.5f, -0.5f, -0.5f,
   //   1.0f, 0.0f, 0.0f,//4 color
   //   0.5f, -0.5f, -0.5f,
   //   0.0f, 1.0f, 0.0f, //5 color
   //   0.5f,  0.5f, -0.5f,
   //   0.0f, 0.0f, 1.0f,//6 color
   //   -0.5f,  0.5f, -0.5f,
   //   0.2f, 0.6f, 0.0f //7 color
   //};

   //GLuint indices[] = {
   //   // Stripe 1
   //   6, 5, 7, 4, // back
   //   3, 0, // left
   //   2, 1, // front
   //   6, 5, // right

   //         // Stripe 2
   //         // bottom
   //   5, 1, 4, 0,

   //   // front
   //   0, 1, 2,
   //   2, 0, 3,

   //   // top
   //   3, 2, 6,
   //   6, 3, 7
   //};
}

CSkyBox::CSkyBox()
   : CCube()
{
   mGeometry.reset(new CGeometry());

   mGeometry->setVertexBuffer(vertices);
   const int numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
   mGeometry->setNumOfVertices(numOfVertices);

   mGeometry->setIndexBuffer(indices);
   const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
   mGeometry->setNumOfIndices(numOfIndices);

   ///@todo: get rid of the magic numbers
   mGeometry->setNumOfElementsPerVertex(3); ///@todo: may be 5
   mGeometry->setVertexStride(5); //5

   /*mGeometry->setVertexBuffer(cubeData);
   const int numOfVertices = sizeof(cubeData) / sizeof(cubeData[0]);
   mGeometry->setNumOfVertices(numOfVertices);

   mGeometry->setIndexBuffer(indices);
   const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
   mGeometry->setNumOfIndices(numOfIndices);

   ///@todo: get rid of the magic numbers
   mGeometry->setNumOfElementsPerVertex(CCube::mNumOfElementsPerVertex);
   mGeometry->setVertexStride(CCube::mStride);*/
}


CSkyBox::~CSkyBox()
{
}

