#include "CLand.h"
#include "CGeometry.h"
#include "CLog.h"
#include "CTexture.h"

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat vertices[] =
   {
     -0.5f, 0.0f, -0.5f, 0.0f, 1.0f,
      -0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
      0.5f, 0.0f, -0.5f, 1.0f, 1.0f,
      0.5f, 0.0f, 0.5f, 1.0f, 0.0f
   };

   GLuint indices[] =
   {
      0, 1, 2, 3
   };
}

CLand::CLand()
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

      mGeometry->setNumOfElementsPerVertex(3);
      mGeometry->setVertexStride(5);
   }
}

CLand::~CLand()
{
}

void CLand::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().logGL("\n** CLand::setupShadersAndBuffers() **");

   // Shader setup
   assert(pShader);
   mShader = pShader;

   if (mGeometry)
   {
      // VBO
      glGenBuffers(1, &mVboId);
      glBindBuffer(GL_ARRAY_BUFFER, mVboId);
      CLog::getInstance().logGL("* CLand glBindBuffer() VBO: ");

      GLuint* data = static_cast<GLuint*>(mGeometry->getVertexBuffer());
      glBufferData(GL_ARRAY_BUFFER, mGeometry->getNumOfVertices()* sizeof(GLuint), data, GL_STATIC_DRAW);

      // Index buffer
      glGenBuffers(1, &mIboId);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboId);
      CLog::getInstance().logGL("* CLand glBindBuffer() index buffer: ");

      GLuint* indices = (GLuint*)mGeometry->getIndexBuffer();
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, mGeometry->getNumOfIndices()* sizeof(GLuint), indices, GL_STATIC_DRAW);

      // Reset VBOs
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }
}

bool CLand::loadTexture(const char * fileName)
{
   return (0 != mTexture->loadBmpTexture(fileName));
}
