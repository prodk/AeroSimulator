#include "CCube.h"
#include "CGeometry.h"
#include "CSimpleShader.h"
#include "CLog.h"

///@todo: think which headers are necessary
#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/wglew.h"
#include <gl/GL.h>
#include "../AeroSimulator/include/glext.h"
#include <cassert>

using namespace AeroSimulatorEngine;

namespace
{
   // Unit cube
   GLfloat cubeData[] = {
      //front
      -0.5f, -0.5f,  0.5f,
      1.0f, 0.0f, 0.0f, //0 color
      0.5f, -0.5f,  0.5f,
      0.0f, 1.0f, 0.0f, //1 color
      0.5f,  0.5f,  0.5f,
      0.0f, 0.0f, 1.0f, //2 color
      -0.5f,  0.5f,  0.5f,
      0.2f, 0.0f, 0.5f, //3 color
       // back
      -0.5f, -0.5f, -0.5f,
      1.0f, 0.0f, 0.0f,//4 color
      0.5f, -0.5f, -0.5f,
      0.0f, 1.0f, 0.0f, //5 color
      0.5f,  0.5f, -0.5f,
      0.0f, 0.0f, 1.0f,//6 color
      -0.5f,  0.5f, -0.5f,
      0.2f, 0.6f, 0.0f //7 color
   };

   ///@todo: optimize the inidices
   ///@todo: this strip is not correct: it should be bottom->front->top, 2nd front is missing
   GLuint indices[] = {
      6, 5, 7, 4, // back
      3, 0, // left
      2, 1, // front
      6, 5, // right

      5, 4, 1, 0, // bottom
      2, 3, 6, 7  // top
   };

   //GLuint indices[] = {
   //   // Stripe 1
   //   6, 5, 7, 4, // back
   //    3, 0, // left
   //    2, 1, // front
   //    6, 5, // right

   //   // Stripe 2
   //   // bottom
   //   5, 1, 4, 0,

   //   // front
   //   0, 1, 2,
   //   2, 0, 3,

   //   // top
   //   3, 2, 6,
   //   6, 3, 7
   //};
}

CCube::CCube()
   /*mGeometry(new CGeometry())
   , mShader(new CSimpleShader())*/
{
   /*setGeometry(mGeometry.get());
   setShader(mShader.get());*/
   mGeometry.reset(new CGeometry());
   mShader.reset(new CSimpleShader());

   assert(mGeometry.get());
   assert(mShader.get());
}

CCube::~CCube()
{
}

void CCube::setupGeometry()
{
   mGeometry->setVertexBuffer(cubeData);
   const int numOfVertices = sizeof(cubeData) / sizeof(cubeData[0]);
   mGeometry->setNumOfVertices(numOfVertices);

   mGeometry->setIndexBuffer(indices);
   const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
   mGeometry->setNumOfIndices(numOfIndices);


   ///@todo: make these magic number static consts
   mGeometry->setNumOfElementsPerVertex(3); // 3 coordinates/color components per vertex
   mGeometry->setVertexStride(6); // stride of 6 for 3 coordinates and 3 colors
}

void CCube::setupVBO()
{
   // Shader setup
   mShader->link();
   mShader->setup(*this);

   ///@todo: these too, move inside CRenderable, just call setupVBO(); vbo/ibo ids also must be inside CRenderable
   // VBO
   glGenBuffers(1, &mVboId);
   glBindBuffer(GL_ARRAY_BUFFER, mVboId);
   CLog::getInstance().logGL("* glBindBuffer() VBO: ");

   GLuint* data = static_cast<GLuint*>(mGeometry->getVertexBuffer());
   glBufferData(GL_ARRAY_BUFFER, mGeometry->getNumOfVertices()* sizeof(GLuint), data, GL_STATIC_DRAW);

   // Index buffer
   glGenBuffers(1, &mIboId);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboId);
   CLog::getInstance().log("* glBindBuffer() index buffer: ");

   GLuint* indices = (GLuint*)mGeometry->getIndexBuffer();
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mGeometry->getNumOfIndices()* sizeof(GLuint), indices, GL_STATIC_DRAW);

   ///@todo: add resetting glBindBUffer here!
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
