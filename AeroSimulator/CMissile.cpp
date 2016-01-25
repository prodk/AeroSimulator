#include "CMissile.h"
#include "CGeometry.h"
#include "CParticleSystem.h"
#include "CWin32Renderer.h"
#include "CCommonMath.h"

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
   , mSpeed(glm::vec3(30.0f, 500.0f, 30.0f))
   //, mSpeed(glm::vec3(10.0f, 100.0f, 10.0f))
   , mFire(nullptr)
{
   //mGeometry.reset(new CGeometry());

   //assert(mGeometry);

   //if (mGeometry)
   //{
   //   mGeometry->setVertexBuffer(vertices);
   //   const int numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
   //   mGeometry->setNumOfVertices(numOfVertices);

   //   mGeometry->setIndexBuffer(indices);
   //   const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
   //   mGeometry->setNumOfIndices(numOfIndices);

   //   ///@todo: get rid of the magic numbers
   //   mGeometry->setNumOfElementsPerVertex(3);
   //   mGeometry->setVertexStride(5); // Vertex + texture coordinates
   //}
}

CMissile::~CMissile()
{
}

void CMissile::update(float dt)
{
   ///@todo: it is much better to introduce a position in the world space and use it here
   //glm::vec3 currentPos = getTranslate();
   //currentPos -= mFlightDirection * mSpeed *glm::vec3(dt, 0.0f, dt);
   //currentPos.y -= mSpeed.y * dt * dt; // Gravity
   //setTranslate(currentPos);

   //calculateModelMatrix();

   //mFire->update(dt);
   //mFire->resetParentTRMatrix();

   //mFire->setTranslate(glm::vec3(0.0f, 0.0f, 0.5f));
   //mFire->buildModelMatrix(getModelMatrix());
}

//void CMissile::addParticles(std::shared_ptr<CShader>& pShader, 
//                            std::shared_ptr<CShader>& pColorShader,
//                            const char * filePath, const glm::vec2 & frameSize, const float width, const float height)
//{
//   mFire.reset(new CParticleSystem(2.0f, 4.0f, 16, glm::vec3(0.0f, 0.0f, 3.5f)));
//   //if (mFire)
//   //{
//   //   mFire->setTranslate(glm::vec3(0.0f, 0.0f, 0.1f));
//   //   mFire->addParticles(pShader, pColorShader, filePath, glm::vec2(4.0f, 4.0f), 1.0f, 1.0f);
//
//   //   // Add fire to the airplane
//   //   add(mFire.get());
//
//   //   buildModelMatrix(glm::mat4x4(1.0f));
//   //}
//}

void CMissile::setEmitSpeed(const float factor)
{
   mFire->setEmitSpeed(factor);
}

void CMissile::resetEmitSpeed()
{
   mFire->resetEmitSpeed();
}

void CMissile::setFireVisible(const bool visible)
{
   mFire->showParticles(visible);
}
