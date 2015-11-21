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
   //, mObjectTree(new CParentGameObject())
   , mCabine(new CParentGameObject())
   , mBody(new CParentGameObject())
   , mLeftWing(new CParentGameObject())
   , mRightWing(new CParentGameObject())
   , mTail(new CParentGameObject())
   , mPropeller(new CParentGameObject()) ///@todo: make this a separate class which modifies its TR matrix in updateTRMatrix()
   , mCubes(numOfCubes)
{
   assert(mCubeGeometry);
   //assert(mObjectTree);
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
      mCubes[count].setGeometry(mCubeGeometry);
   }

   // Build an airplane
   // Cabine is at (0., 0., 0.) and contains 1 Cube
   mCabine->add(&mCubes[0]);
   mCubes[0].scale(glm::vec3(0.5f, 0.5f, 0.4f));

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

   // Build the model matrix of each node of the tree
   mCabine->buildModelMatrix(glm::mat4x4(1.0f));

   //mObjectTree->add(mCabine.get());

   ////mCubes[0].resetTRMatrix(mObjectTree->getTRMatrix());
   //mCubes[0].setParentModelMatrix(mObjectTree->getTRMatrix());
   //mCubes[0].scale(glm::vec3(0.5f, 0.5f, 0.4f));
   //mCabine->add(&mCubes[0]); // cube[0] is a Cabin.
   //mCabine->add(mBody.get());

   //buildBody();

   //// Wings are children of the Body
   ////Left wing
   //mBody->add(mLeftWing.get());

   ////mLeftWing->resetTRMatrix(mBody->getTRMatrix());
   //mLeftWing->setParentModelMatrix(mBody->getParentModelMatrix());
   ////mCubes[5].resetModelMatrix(mLeftWing->getTRMatrix());
   //mCubes[5].setParentModelMatrix(mLeftWing->getParentModelMatrix());
   //mCubes[5].translate(glm::vec3(-1.0f, -0.75f, 1.5f));
   //mCubes[5].scale(glm::vec3(2.0f, 0.3f, 1.5f));
   //mLeftWing->add(&mCubes[5]);

   ////Right wing
   //mBody->add(mRightWing.get());

   //mRightWing->resetTRMatrix(mBody->getTRMatrix());
   //mCubes[6].resetModelMatrix(mRightWing->getTRMatrix());
   //mCubes[6].translate(glm::vec3(1.0f, -0.75f, 1.5f));
   //mCubes[6].scale(glm::vec3(2.0f, 0.3f, 1.5f));
   //mRightWing->add(&mCubes[6]);

   //// Tail is a child of the Body
   //mBody->add(mTail.get());
   //mTail->resetTRMatrix(mBody->getTRMatrix());

   //// Tail cube 1
   //mCubes[7].resetTRMatrix(mTail->getTRMatrix());
   //const float shift = 0.5f + std::sqrtf(2.0f)*0.5f;
   //mCubes[7].translate(glm::vec3(0.0f, -0.15f, (2.8f + shift)));
   //mCubes[7].rotate(glm::vec3(45.0f, 0.0f, 0.0f));
   //mCubes[7].scale(glm::vec3(0.5f, 1.0f, 0.5f));
   //mTail->add(&mCubes[7]);

   //// Tail cube 2
   //mCubes[8].resetTRMatrix(mTail->getTRMatrix());
   //mCubes[8].translate(glm::vec3(0.0f, 0.57f, 2.7f + shift + 0.8f));
   //mCubes[8].rotate(glm::vec3(45.0f, 0.0f, 0.0f));
   //mCubes[8].scale(glm::vec3(0.5f, 1.0f, 0.5f));
   //mTail->add(&mCubes[8]);

   //// Propeller is a child of the Body
   //mBody->add(mPropeller.get());
   ////mPropeller->resetTRMatrix(mTail->getTRMatrix());
   ////mPropeller->resetTRMatrix(mBody->getChildTRMatrix(0));

   //// Base
   ////mCubes[9].resetTRMatrix(mPropeller->getTRMatrix());
   //mCubes[9].setParentModelMatrix(mBody->getParentModelMatrix()*mBody->getChildTRMatrix(1));
   //mCubes[9].translate(glm::vec3(0.0f, 0.f, -0.61275f));
   //mCubes[9].scale(glm::vec3(0.25f, 0.25f, 0.9f));
   //mPropeller->add(&mCubes[9]);

   //// Paddle 1
   ////mCubes[10].resetTRMatrix(mPropeller->getChildTRMatrix(0));
   //mCubes[10].setParentModelMatrix(mPropeller->getChildTRMatrix(0));
   //mCubes[10].translate(glm::vec3(-0.7f, 0.f, -0.9f));
   //mCubes[10].scale(glm::vec3(1.2f, 0.25f, 0.1f));
   //mPropeller->add(&mCubes[10]);

   //// Paddle 2
   ////mCubes[11].resetTRMatrix(mPropeller->getTRMatrix());
   //mCubes[11].setParentModelMatrix(mPropeller->getChildTRMatrix(0));
   //mCubes[11].translate(glm::vec3(0.7f, 0.f, -0.9f));
   //mCubes[11].scale(glm::vec3(1.2f, 0.25f, 0.1f));
   //mPropeller->add(&mCubes[11]);

   //// Force the propeller to rotate
   //mPropeller->setDynamic();

   // Fans are children of the Tail

   return result;
}

void C3DModel::getTree(std::vector<CGameObject*>& tree) const
{
   //mObjectTree->traverse(tree);
   mCabine->traverse(tree);
}
