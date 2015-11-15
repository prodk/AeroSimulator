#include "CCube.h"
#include "CGeometry.h"
#include "CSimpleShader.h"
#include "CLog.h"
#include "CCommonMath.h"

///@todo: think which headers are necessary
#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/wglew.h"
#include <gl/GL.h>
#include "../AeroSimulator/include/glext.h"
#include <cassert>

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

const int CCube::mNumOfElementsPerVertex = 3; // 3 coordinates/color components per vertex
const int CCube::mStride = 6;                 // stride of 6 for 3 coordinates and 3 colors

CCube::CCube()
   : CGameObject()
{
   mIsLeaf = true;

   ///@todo: remove this incorrect code
   mShader.reset(new CSimpleShader());

   //assert(mGeometry.get());
   assert(mShader.get());

   mScale = glm::vec3(1.0f, 1.0f, 1.0f);

   CLog::getInstance().log("\n* CCube::CCube() default: success. \n");
}

CCube::~CCube()
{
   mGeometry.reset();
   mShader.reset();
}

CCube::CCube(const CGameObject* parent,
             const glm::vec3 & scale,
             const glm::vec3 & rotate,
             const glm::vec3 & translate)
   : CGameObject(parent, scale, rotate, translate)
{
   mIsLeaf = true;

   ///@todo: this is incorrect! We must keep only a reference to the Geometry and shader!
   mShader.reset(new CSimpleShader());

   //assert(mGeometry.get());
   assert(mShader.get());

   // Set transforms
   glm::mat4 myModel = glm::mat4(1.0f);

   // TRS
   // scale
   myModel = glm::scale(myModel, scale);

   // rotate
   const float angleX = CCommonMath::degToRad(rotate.x);
   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   myModel = glm::rotate(myModel, angleX, xAxis);

   const float angleY = CCommonMath::degToRad(rotate.y);
   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   myModel = glm::rotate(myModel, angleY, yAxis);

   const float angleZ = CCommonMath::degToRad(rotate.z);
   glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
   myModel = glm::rotate(myModel, angleZ, zAxis);

   // translate
   myModel = glm::translate(myModel, translate);

   ///@todo: probably myltiply by the parent matrix
   glm::mat4 parentModelMatrix = glm::mat4(1.0f);
   if (parent)
   {
      parentModelMatrix = parent->getModelMatrix();
   }

   mModelMatrix = parentModelMatrix * myModel;

   CLog::getInstance().log("\n* CCube::CCube() non-default: success. \n");
}

void CCube::setupGeometry(std::shared_ptr<CGeometry>& pGeometry)
{
   mGeometry = pGeometry;
}

void CCube::setupShadersAndBuffers()
{
   CLog::getInstance().logGL("\n** CCube::setupVBO() **");

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

void CCube::add(CGameObject * child)
{
   CLog::getInstance().log("\n!!! Cannot add a child to CCube because it is a leaf!!! \n");
}

void CCube::traverse(std::vector<CGameObject*>& tree)
{
}
