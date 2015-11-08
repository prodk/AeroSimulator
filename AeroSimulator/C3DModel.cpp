#include "C3DModel.h"
#include "CParentGameObject.h"
#include "CCube.h"

#include <cassert>

using namespace AeroSimulatorEngine;

const std::size_t C3DModel::numOfCubes = 12;

C3DModel::C3DModel()
   : mObjectTree(new CParentGameObject())
   , mBody(new CParentGameObject())
   , mLeftWing(new CParentGameObject())
   , mRightWing(new CParentGameObject())
   , mTail(new CParentGameObject())
   , mPropeller(new CParentGameObject())
   , mCubes(numOfCubes)
{
   assert(mObjectTree);
}

C3DModel::~C3DModel()
{
}

bool C3DModel::buildModel()
{
   bool result = false;

   // Build an airplane
   mCubes[0].resetModelMatrix(mObjectTree->getModelMatrix());
   mCubes[0].scale(glm::vec3(1.0f, 0.5f, 1.f));
   mObjectTree->add(&mCubes[0]); // cube[0] is a Cabin.
   mObjectTree->add(mBody.get());

   buildBody();

   // Wings are children of the Body
   //Left wing
   mBody->add(mLeftWing.get());

   mLeftWing->resetModelMatrix(mBody->getModelMatrix());
   mCubes[5].resetModelMatrix(mLeftWing->getModelMatrix());
   mCubes[5].translate(glm::vec3(-1.0f, -1.0f, 1.5f));
   mCubes[5].scale(glm::vec3(2.0f, 0.5f, 1.5f));
   mLeftWing->add(&mCubes[5]);

   //Right wing
   mBody->add(mRightWing.get());

   mRightWing->resetModelMatrix(mBody->getModelMatrix());
   mCubes[6].resetModelMatrix(mRightWing->getModelMatrix());
   mCubes[6].translate(glm::vec3(1.0f, -1.0f, 1.5f));
   mCubes[6].scale(glm::vec3(2.0f, 0.5f, 1.5f));
   mRightWing->add(&mCubes[6]);

   // Tail is a child of the Body
   mBody->add(mTail.get());
   mTail->resetModelMatrix(mBody->getModelMatrix());

   // Tail cube 1
   mCubes[7].resetModelMatrix(mTail->getModelMatrix());
   mCubes[7].translate(glm::vec3(0.0f, -0.3f, 3.5f));
   mCubes[7].rotate(glm::vec3(45.0f, 0.0f, 0.0f));
   mCubes[7].scale(glm::vec3(0.5f, 1.0f, 1.0f));
   mTail->add(&mCubes[7]);

   // Tail cube 2
   mCubes[8].resetModelMatrix(mTail->getModelMatrix());
   mCubes[8].translate(glm::vec3(0.0f, 0.7f, 3.5f));
   mCubes[8].rotate(glm::vec3(45.0f, 0.0f, 0.0f));
   mCubes[8].scale(glm::vec3(0.5f, 1.0f, 1.0f));
   mTail->add(&mCubes[8]);

   // Propeller is a child of the Body


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
   mBody->resetModelMatrix(mObjectTree->getModelMatrix());
   // cube 1
   mCubes[1].resetModelMatrix(mBody->getModelMatrix());
   mCubes[1].translate(glm::vec3(0.0f, -1.0f, 0.0f));
   mBody->add(&mCubes[1]);

   // cube 2
   mCubes[2].resetModelMatrix(mBody->getModelMatrix());
   mCubes[2].translate(glm::vec3(0.0f, -1.0f, 1.0f));
   mBody->add(&mCubes[2]);

   // cube 3
   mCubes[3].resetModelMatrix(mBody->getModelMatrix());
   mCubes[3].translate(glm::vec3(0.0f, -1.0f, 2.0f));
   mBody->add(&mCubes[3]);

   // cube 4
   mCubes[4].resetModelMatrix(mBody->getModelMatrix());
   mCubes[4].translate(glm::vec3(0.0f, -1.0f, 3.0f));
   mBody->add(&mCubes[4]);
}
