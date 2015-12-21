#include "CMissile.h"
#include "CGeometry.h"

using namespace AeroSimulatorEngine;

///@todo: add geometry to CCube, remove from C3DModel
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
}

CMissile::CMissile()
   : mIsDetached(false)
   , mFlightDirection()
   , mSpeed(glm::vec3(20.0f, 140.0f, 20.0f))
{
   mGeometry.reset(new CGeometry());

   assert(mGeometry);

   if (mGeometry)
   {
      mGeometry->setVertexBuffer(vertices);
      const int numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
      mGeometry->setNumOfVertices(numOfVertices);

      mGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mGeometry->setNumOfIndices(numOfIndices);

      ///@todo: get rid of the magic numbers
      mGeometry->setNumOfElementsPerVertex(3);
      mGeometry->setVertexStride(5); // Vertex + texture coordinates
   }
}

CMissile::~CMissile()
{
}

void CMissile::update(float dt)
{
   glm::vec3 currentPos = getTranslate();
   currentPos -= mFlightDirection * mSpeed *glm::vec3(dt, 0.0f, dt);
   currentPos.y -= mSpeed.y * dt * dt; // Gravity
   setTranslate(currentPos);
}
