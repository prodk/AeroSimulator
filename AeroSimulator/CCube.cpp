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

#include "glm/gtc/matrix_transform.hpp"

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

const int CCube::mNumOfElementsPerVertex = 3; // 3 coordinates/color components per vertex
const int CCube::mStride = 6;                 // stride of 6 for 3 coordinates and 3 colors

CCube::CCube()
   : CGameObject()
{
   mGeometry.reset(new CGeometry());
   mShader.reset(new CSimpleShader());

   assert(mGeometry.get());
   assert(mShader.get());
}

CCube::~CCube()
{
}

CCube::CCube(const glm::mat4 & parentModelMatrix,
             const glm::vec3 & scale,
             const glm::vec3 & rotate,
             const glm::vec3 & translate)
   : CGameObject(parentModelMatrix, scale, rotate, translate)
{
   mGeometry.reset(new CGeometry());
   mShader.reset(new CSimpleShader());

   assert(mGeometry.get());
   assert(mShader.get());

   // Set transforms
   glm::mat4 myModel = glm::mat4(1.0f);

   // TRS
   // scale
   myModel = glm::scale(myModel, scale);

   // rotate
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   myModel = glm::rotate(myModel, rotate.x, xAxis);

   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   myModel = glm::rotate(myModel, rotate.y, yAxis);

   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   myModel = glm::rotate(myModel, rotate.z, zAxis);

   myModel = glm::translate(myModel, translate);

   ///@todo: probably myltiply by the parent matrix
   setModelMatrix(myModel);
}

void CCube::setupGeometry()
{
   mGeometry->setVertexBuffer(cubeData);
   const int numOfVertices = sizeof(cubeData) / sizeof(cubeData[0]);
   mGeometry->setNumOfVertices(numOfVertices);

   mGeometry->setIndexBuffer(indices);
   const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
   mGeometry->setNumOfIndices(numOfIndices);

   mGeometry->setNumOfElementsPerVertex(mNumOfElementsPerVertex);
   mGeometry->setVertexStride(mStride);
}

void CCube::setupVBO()
{
   // Shader setup
   mShader->link();
   mShader->setup(*this);

   // VBO
   glGenBuffers(1, &mVboId);
   glBindBuffer(GL_ARRAY_BUFFER, mVboId);
   CLog::getInstance().logGL("* glBindBuffer() VBO: ");

   GLuint* data = static_cast<GLuint*>(mGeometry->getVertexBuffer());
   glBufferData(GL_ARRAY_BUFFER, mGeometry->getNumOfVertices()* sizeof(GLuint), data, GL_STATIC_DRAW);

   // Index buffer
   glGenBuffers(1, &mIboId);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboId);
   CLog::getInstance().logGL("* glBindBuffer() index buffer: ");

   GLuint* indices = (GLuint*)mGeometry->getIndexBuffer();
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mGeometry->getNumOfIndices()* sizeof(GLuint), indices, GL_STATIC_DRAW);

   // Reset VBOs
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
