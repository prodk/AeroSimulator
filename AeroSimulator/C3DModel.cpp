#include "C3DModel.h"
#include "CParentGameObject.h"
#include "CCube.h"
#include "CGeometry.h"
#include "CPropeller.h"
#include "CAxesFrame.h"

#include <cassert>

using namespace AeroSimulatorEngine;

const std::size_t C3DModel::numOfCubes = 14;

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
      6, 3, 7
   };
}

C3DModel::C3DModel()
   : mCubeGeometry(new CGeometry())
   , mCabine(new CParentGameObject())
   , mBody(new CParentGameObject())
   , mLeftWing(new CParentGameObject())
   , mRightWing(new CParentGameObject())
   , mTail(new CParentGameObject())
   , mPropeller(new CPropeller())
   , mCubes(numOfCubes)
   , mAxes(new CAxesFrame())
{
   assert(mCubeGeometry);
   assert(mCabine);
   assert(mBody);
   assert(mLeftWing);
   assert(mRightWing);
   assert(mTail);
   assert(mPropeller);
}

C3DModel::~C3DModel()
{
}

void  C3DModel::setupCubeGeometry()
{
   if (mCubeGeometry)
   {
      mCubeGeometry->setVertexBuffer(cubeData);
      const int numOfVertices = sizeof(cubeData) / sizeof(cubeData[0]);
      mCubeGeometry->setNumOfVertices(numOfVertices);

      mCubeGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mCubeGeometry->setNumOfIndices(numOfIndices);

      mCubeGeometry->setNumOfElementsPerVertex(CCube::mNumOfElementsPerVertex);
      mCubeGeometry->setVertexStride(CCube::mStride);
   }
}

bool C3DModel::buildModel()
{
   bool result = false;

   /// Use only one cube geometry!
   setupCubeGeometry();

   // Force all the cubes to use one geometry
   for (std::size_t count = 0u; count < mCubes.size(); ++count)
   {
      mCubes[count].setGeometry(mCubeGeometry);
   }

   // Build an airplane
   // Cabine is at (0., 0., 0.) and contains 1 Cube
   mCabine->add(&mCubes[0]);
   mCubes[0].scale(glm::vec3(0.5f, 0.5f, 0.4f));

   mAxes->setTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
   mCabine->add(mAxes.get());

   /// The Body
   // Body is shifted relative to the cabine has several cubes
   mCabine->add(mBody.get());
   mBody->setTranslate(glm::vec3(0.0f, -0.75f, 0.0f));
   // Cubes of the body
   // Cube 1 coincides with the body
   mBody->add(&mCubes[1]);
   // Cubes 2-4 are shifted
   mCubes[2].setTranslate(glm::vec3(0.0f, 0.0f, 1.0f));
   mCubes[2].setScale(glm::vec3(1.0f, 0.5f, 1.0f));
   mBody->add(&mCubes[2]);
   mCubes[3].setTranslate(glm::vec3(0.0f, 0.0f, 2.0f));
   mBody->add(&mCubes[3]);
   mCubes[4].setTranslate(glm::vec3(0.0f, 0.0f, 3.0f));
   mBody->add(&mCubes[4]);

   /// The Wings.
   // Wings are children of the Body
   const float wingX = 1.8f;
   // Left wing
   mBody->add(mLeftWing.get());
   mLeftWing->setTranslate(glm::vec3(-1.0f - 0.5f*(wingX - 1.0f), 0.0f, 1.25f));
   mCubes[5].setScale(glm::vec3(wingX, 0.1f, 1.1f));
   mLeftWing->add(&mCubes[5]);

   // Right wing
   mBody->add(mRightWing.get());
   mRightWing->setTranslate(glm::vec3(1.0f + 0.5f*(wingX - 1.0f), 0.0f, 1.25f));
   mCubes[6].setScale(glm::vec3(wingX, 0.1f, 1.1f));
   mRightWing->add(&mCubes[6]);

   /// The Propeller - it is dynamic, so is a special class
   // Propeller is a child of the Body
   mBody->add(mPropeller.get());
   mPropeller->setTranslate(glm::vec3(0.0f, 0.0f, -0.75f));

   mCubes[7].setScale(glm::vec3(0.2f, 0.25f, 0.5f));
   mPropeller->add(&mCubes[7]);

   mCubes[8].setTranslate(glm::vec3(0.0f, 0.75f, -0.25f));
   mCubes[8].setScale(glm::vec3(0.1f, 1.5f, 0.1f));
   mPropeller->add(&mCubes[8]);

   mCubes[9].setTranslate(glm::vec3(0.0f, -0.75f, -0.25f));
   mCubes[9].setScale(glm::vec3(0.1f, 1.5f, 0.1f));
   mPropeller->add(&mCubes[9]);

   /// The Tail.
   mBody->add(mTail.get());
   mTail->setTranslate(glm::vec3(0.0f, 0.37*sqrtf(2.0f), 3.75f));
   mTail->setRotate(glm::vec3(-45.0f, 0.0f, 0.0f));

   mCubes[10].setScale(glm::vec3(0.25f, 0.5f, 0.5f));
   mTail->add(&mCubes[10]);

   mCubes[11].setTranslate(glm::vec3(0.0f, 0.f, 0.5f));
   mCubes[11].setScale(glm::vec3(0.25f, 0.5f, 0.5f));
   mTail->add(&mCubes[11]);

   /// Fans are children of the tail
   // Left fan
   mCubes[12].setTranslate(glm::vec3(-0.4f, 0.f, 0.5f));
   mCubes[12].setRotate(glm::vec3(45.0f, 0.0f, 0.0f));
   mCubes[12].setScale(glm::vec3(1.0f, 0.1f, 0.5f));
   mTail->add(&mCubes[12]);

   // Right fan
   mCubes[13].setTranslate(glm::vec3(0.4f, 0.f, 0.5f));
   mCubes[13].setRotate(glm::vec3(45.0f, 0.0f, 0.0f));
   mCubes[13].setScale(glm::vec3(1.0f, 0.1f, 0.5f));
   mTail->add(&mCubes[13]);

   // Build the model matrix of each node of the tree
   mCabine->buildModelMatrix(glm::mat4x4(1.0f));

   result = true;
   return result;
}

void C3DModel::getTree(std::vector<CCompositeGameObject*>& tree) const
{
   mCabine->traverse(tree);
}
