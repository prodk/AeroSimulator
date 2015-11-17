#include "C3DModel.h"
#include "CParentGameObject.h"
#include "CCube.h"
#include "CGeometry.h"

#include <cassert>

using namespace AeroSimulatorEngine;

const std::size_t C3DModel::numOfCubes = 12;

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
   , mObjectTree(new CParentGameObject())
   , mCabine(new CParentGameObject())
   , mBody(new CParentGameObject())
   , mLeftWing(new CParentGameObject())
   , mRightWing(new CParentGameObject())
   , mTail(new CParentGameObject())
   , mPropeller(new CParentGameObject())
   , mCubes(numOfCubes)
{
   assert(mCubeGeometry);
   assert(mObjectTree);
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

      ///@todo: get rid of the magic numbers
      mCubeGeometry->setNumOfElementsPerVertex(CCube::mNumOfElementsPerVertex);
      mCubeGeometry->setVertexStride(CCube::mStride);
   }
}

bool C3DModel::buildModel()
{
   bool result = false;

   /// Use only one cube geometry!
   setupCubeGeometry();

   // Force all the cubes to use one geometry.
   for (std::size_t count = 0u; count < mCubes.size(); ++count)
   {
      mCubes[count].setupGeometry(mCubeGeometry);
   }

   // Build an airplane
   mObjectTree->add(mCabine.get());

   //mCubes[0].resetTRMatrix(mObjectTree->getTRMatrix());
   mCubes[0].setParentModelMatrix(mObjectTree->getTRMatrix());
   mCubes[0].scale(glm::vec3(0.5f, 0.5f, 0.4f));
   mCabine->add(&mCubes[0]); // cube[0] is a Cabin.
   mCabine->add(mBody.get());

   buildBody();

   // Wings are children of the Body
   //Left wing
   mBody->add(mLeftWing.get());

   //mLeftWing->resetTRMatrix(mBody->getTRMatrix());
   mLeftWing->setParentModelMatrix(mBody->getParentModelMatrix());
   //mCubes[5].resetModelMatrix(mLeftWing->getTRMatrix());
   mCubes[5].setParentModelMatrix(mLeftWing->getParentModelMatrix());
   mCubes[5].translate(glm::vec3(-1.0f, -0.75f, 1.5f));
   mCubes[5].scale(glm::vec3(2.0f, 0.3f, 1.5f));
   mLeftWing->add(&mCubes[5]);

   //Right wing
   mBody->add(mRightWing.get());

   mRightWing->resetTRMatrix(mBody->getTRMatrix());
   mCubes[6].resetModelMatrix(mRightWing->getTRMatrix());
   mCubes[6].translate(glm::vec3(1.0f, -0.75f, 1.5f));
   mCubes[6].scale(glm::vec3(2.0f, 0.3f, 1.5f));
   mRightWing->add(&mCubes[6]);

   // Tail is a child of the Body
   mBody->add(mTail.get());
   mTail->resetTRMatrix(mBody->getTRMatrix());

   // Tail cube 1
   mCubes[7].resetTRMatrix(mTail->getTRMatrix());
   const float shift = 0.5f + std::sqrtf(2.0f)*0.5f;
   mCubes[7].translate(glm::vec3(0.0f, -0.15f, (2.8f + shift)));
   mCubes[7].rotate(glm::vec3(45.0f, 0.0f, 0.0f));
   mCubes[7].scale(glm::vec3(0.5f, 1.0f, 0.5f));
   mTail->add(&mCubes[7]);

   // Tail cube 2
   mCubes[8].resetTRMatrix(mTail->getTRMatrix());
   mCubes[8].translate(glm::vec3(0.0f, 0.57f, 2.7f + shift + 0.8f));
   mCubes[8].rotate(glm::vec3(45.0f, 0.0f, 0.0f));
   mCubes[8].scale(glm::vec3(0.5f, 1.0f, 0.5f));
   mTail->add(&mCubes[8]);

   // Propeller is a child of the Body
   mBody->add(mPropeller.get());
   //mPropeller->resetTRMatrix(mTail->getTRMatrix());
   //mPropeller->resetTRMatrix(mBody->getChildTRMatrix(0));

   // Base
   //mCubes[9].resetTRMatrix(mPropeller->getTRMatrix());
   mCubes[9].setParentModelMatrix(mBody->getParentModelMatrix()*mBody->getChildTRMatrix(1));
   mCubes[9].translate(glm::vec3(0.0f, 0.f, -0.61275f));
   mCubes[9].scale(glm::vec3(0.25f, 0.25f, 0.9f));
   mPropeller->add(&mCubes[9]);

   // Paddle 1
   //mCubes[10].resetTRMatrix(mPropeller->getChildTRMatrix(0));
   mCubes[10].setParentModelMatrix(mPropeller->getChildTRMatrix(0));
   mCubes[10].translate(glm::vec3(-0.7f, 0.f, -0.9f));
   mCubes[10].scale(glm::vec3(1.2f, 0.25f, 0.1f));
   mPropeller->add(&mCubes[10]);

   // Paddle 2
   //mCubes[11].resetTRMatrix(mPropeller->getTRMatrix());
   mCubes[11].setParentModelMatrix(mPropeller->getChildTRMatrix(0));
   mCubes[11].translate(glm::vec3(0.7f, 0.f, -0.9f));
   mCubes[11].scale(glm::vec3(1.2f, 0.25f, 0.1f));
   mPropeller->add(&mCubes[11]);

   // Force the propeller to rotate
   mPropeller->setDynamic();

   // Fans are children of the Tail

   return result;
}

void C3DModel::getTree(std::vector<CGameObject*>& tree) const
{
   mObjectTree->traverse(tree);
}

void C3DModel::buildBody()
{
   // Body has 4 cubes. Use cubes 1 - 5
   //mBody->resetTRMatrix(mCabine->getChildTRMatrix(0));
   mBody->setParentModelMatrix(mCabine->getParentModelMatrix()*mCabine->getChildTRMatrix(0));

   // Move the body to the position of the first cube
   mBody->translate(glm::vec3(0.0f, 0.0f, 0.0f));

   // cube 1
   //mCubes[1].resetTRMatrix(mBody->getTRMatrix());
   mCubes[1].setParentModelMatrix(mCabine->getParentModelMatrix()*mCabine->getChildTRMatrix(0));
   mCubes[1].translate(glm::vec3(0.0f, -0.75f, 0.0f));
   mBody->add(&mCubes[1]);

   // cube 2
   //mCubes[2].resetTRMatrix(mBody->getChildTRMatrix(0));
   mCubes[2].setParentModelMatrix(mBody->getParentModelMatrix());
   mCubes[2].translate(glm::vec3(0.0f, -0.75f, 1.0f));
   mBody->add(&mCubes[2]);

   // cube 3
   //mCubes[3].resetTRMatrix(mBody->getTRMatrix());
   mCubes[3].setParentModelMatrix(mBody->getParentModelMatrix());
   mCubes[3].translate(glm::vec3(0.0f, -0.75f, 2.0f));
   mBody->add(&mCubes[3]);

   // cube 4
   //mCubes[4].resetTRMatrix(mBody->getTRMatrix());
   mCubes[4].setParentModelMatrix(mBody->getParentModelMatrix());
   mCubes[4].translate(glm::vec3(0.0f, -0.75f, 3.0f));
   mBody->add(&mCubes[4]);
}
