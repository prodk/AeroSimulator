#include "C3DModel.h"
#include "CParentGameObject.h"
#include "CCube.h"
#include "CGeometry.h"
#include "CPropeller.h"
#include "CAxesFrame.h"
#include "CBillBoard.h"
#include "../src/shaders/CShader.h"
#include "CBoundingBox.h"

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

   GLfloat colorCubeData[] = {
      //front
      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,

      // back
      -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
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
   , mAxes(numOfCubes)
   , mBillboardShader()
   , mPosition()
   , mSpeedOfFlight(glm::vec3(3.0f, 10.0f, 3.0f))
   , mBoundingBox(new CBoundingBox())
   , mXzDirection(glm::vec3(0.0f, 0.0f, 1.0f))
   , mFlightAngle(0.0f)
{
   assert(mCubeGeometry);
   assert(mCabine);
   assert(mBody);
   assert(mLeftWing);
   assert(mRightWing);
   assert(mTail);
   assert(mPropeller);
   assert(mBoundingBox);
}

C3DModel::~C3DModel()
{
}

void C3DModel::increasePropellerSpeed()
{
   mPropeller->increaseSpeed();
}

void C3DModel::decreasePropellerSpeed()
{
   mPropeller->decreaseSpeed();
}

void C3DModel::setPropellerSpeed(const float speed)
{
   mPropeller->setSpeed(speed);
}

void C3DModel::resetPropellerSpeed()
{
   mPropeller->setSpeed(100.f);
}

void C3DModel::rotateFlightDirection(const float da, const float dt)
{
   mFlightAngle += da*dt;
   mXzDirection.x = std::sin(mFlightAngle);
   mXzDirection.z = std::cos(mFlightAngle);
}

void C3DModel::resetHealthBars(const float value)
{
   for (auto& cube : mCubes)
      cube.setHealth(value);
}

const CBoundingBox * C3DModel::getBoundingBox() const
{
   if (mBoundingBox)
      return mBoundingBox.get();
   else
      return 0;
}

float C3DModel::getHealth() const
{
   return mCubes[0].getHealthValue();
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

void C3DModel::setupColorCubeGeometry()
{
   if (mCubeGeometry)
   {
      mCubeGeometry->setVertexBuffer(colorCubeData);
      const int numOfVertices = sizeof(colorCubeData) / sizeof(colorCubeData[0]);
      mCubeGeometry->setNumOfVertices(numOfVertices);

      mCubeGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mCubeGeometry->setNumOfIndices(numOfIndices);

      mCubeGeometry->setNumOfElementsPerVertex(CCube::mNumOfElementsPerVertex);
      mCubeGeometry->setVertexStride(3); // 3 coords and nothing else
   }
}

bool C3DModel::buildModel(std::shared_ptr<CShader>& pShader)
{
   bool result = false;
   if (nullptr == pShader)
   {
      return result;
   }
   /// Use only one cube geometry!
   setupColorCubeGeometry();

   // Force all the cubes to use one geometry
   for (std::size_t count = 0u; count < mCubes.size(); ++count)
   {
      mCubes[count].setGeometry(mCubeGeometry);
      // Add debug mode axis to each cube
      mAxes[count].buildModel(pShader);
      mCubes[count].add(&mAxes[count]);
      mCubes[count].setShadersAndBuffers(pShader);

      mCubes[count].setupHealthBar(mBillboardShader);
   }

   // Build an airplane
   /// Bounding box:
   const glm::vec4 bbColor(0.0f, 1.0f, 0.5f, 1.0f);
   const float bbWidth = 4.0f;
   const float bbHeight = 2.0f;
   const float bbDepth = 5.0f;
   mBoundingBox->setColor(bbColor);
   mBoundingBox->setScale(glm::vec3(bbWidth, bbHeight, bbDepth));
   mBoundingBox->setTranslate(glm::vec3(0.0f, -0.2f*bbHeight, 0.4f*bbDepth));
   mBoundingBox->calculateModelMatrix();
   mBoundingBox->setShadersAndBuffers(pShader);
   mCabine->add(mBoundingBox.get());

   /// Cabine.
   const glm::vec4 cabineColor(0.7f, 0.5f, 0.7f, 1.0f);
   // Cabine is at (0., 0., 0.) and contains 1 Cube
   mCabine->add(&mCubes[0]);
   mCubes[0].scale(glm::vec3(0.5f, 0.5f, 0.4f));
   mCubes[0].setColor(cabineColor);
   mCubes[0].translateHealthBar(glm::vec3(0.0f, 0.6f, 0.0f));

   /// The Body
   const glm::vec4 bodyColor(0.5f, 0.2f, 0.9f, 1.0f);
   // Body is shifted relative to the cabine, it has several cubes
   mCabine->add(mBody.get());
   mBody->setTranslate(glm::vec3(0.0f, -0.75f, 0.0f));

   // Cubes of the body
   // Cube 1 coincides with the body
   mCubes[1].setColor(bodyColor);
   mCubes[1].translateHealthBar(glm::vec3(0.5f, 0.7f, 0.0f));
   mBody->add(&mCubes[1]);
   // Cubes 2-4 are shifted
   mCubes[2].setTranslate(glm::vec3(0.0f, 0.0f, 1.0f));
   mCubes[2].setScale(glm::vec3(1.0f, 0.5f, 1.0f));
   mCubes[2].setColor(bodyColor);
   mCubes[2].translateHealthBar(glm::vec3(0.0f, 0.5f, 0.0f));
   mBody->add(&mCubes[2]);

   mCubes[3].setTranslate(glm::vec3(0.0f, 0.0f, 2.0f));
   mCubes[3].setColor(bodyColor);
   mBody->add(&mCubes[3]);

   mCubes[4].setTranslate(glm::vec3(0.0f, 0.0f, 3.0f));
   mCubes[4].setColor(bodyColor);
   mBody->add(&mCubes[4]);

   /// The Wings.
   const glm::vec4 wingColor(1.0f, 0.5f, 0.0f, 1.0f);
   // Wings are children of the Body
   const float wingX = 1.8f;
   // Left wing
   mBody->add(mLeftWing.get());
   mLeftWing->setTranslate(glm::vec3(-1.0f - 0.5f*(wingX - 1.0f), 0.0f, 1.25f));
   mCubes[5].setScale(glm::vec3(wingX, 0.1f, 1.1f));
   mCubes[5].setColor(wingColor);
   mCubes[5].translateHealthBar(glm::vec3(0.0f, 0.3f, 0.0f));
   mLeftWing->add(&mCubes[5]);

   // Right wing
   mBody->add(mRightWing.get());
   mRightWing->setTranslate(glm::vec3(1.0f + 0.5f*(wingX - 1.0f), 0.0f, 1.25f));
   mCubes[6].setScale(glm::vec3(wingX, 0.1f, 1.1f));
   mCubes[6].setColor(wingColor);
   mCubes[6].translateHealthBar(glm::vec3(0.0f, 0.3f, 0.0f));
   mRightWing->add(&mCubes[6]);

   /// The Propeller - it is dynamic, so is a special class
   const glm::vec4 baseColor(1.0f, 0.5f, 0.0f, 1.0f);
   const glm::vec4 paddleColor(1.0f, 1.0f, 0.0f, 1.0f);
   // Propeller is a child of the Body
   mBody->add(mPropeller.get());
   mPropeller->setTranslate(glm::vec3(0.0f, 0.0f, -0.75f));

   mCubes[7].setScale(glm::vec3(0.2f, 0.25f, 0.5f));
   mCubes[7].setColor(baseColor);

   mCubes[7].translateHealthBar(glm::vec3(0.0f, 0.3f, 0.0f));
   mPropeller->add(&mCubes[7]);

   mCubes[8].setTranslate(glm::vec3(0.0f, 0.75f, -0.25f));
   mCubes[8].setScale(glm::vec3(0.1f, 1.5f, 0.1f));
   mCubes[8].setColor(paddleColor);
   mCubes[8].translateHealthBar(glm::vec3(0.0f, 0.0f, -0.3f));
   mPropeller->add(&mCubes[8]);

   mCubes[9].setTranslate(glm::vec3(0.0f, -0.75f, -0.25f));
   mCubes[9].setScale(glm::vec3(0.1f, 1.5f, 0.1f));
   mCubes[9].setColor(paddleColor);
   mCubes[9].translateHealthBar(glm::vec3(0.0f, 0.0f, -0.3f));
   mPropeller->add(&mCubes[9]);

   /// The Tail.
   const glm::vec4 tailColor(1.0f, 0.8, 0.2f, 1.0f);

   mBody->add(mTail.get());
   mTail->setTranslate(glm::vec3(0.0f, 0.37*sqrtf(2.0f), 3.75f));
   mTail->setRotate(glm::vec3(-45.0f, 0.0f, 0.0f));

   mCubes[10].setScale(glm::vec3(0.25f, 0.5f, 0.5f));
   mCubes[10].setColor(tailColor);
   mCubes[10].translateHealthBar(glm::vec3(0.0f, 0.3f, 0.0f));
   mTail->add(&mCubes[10]);

   mCubes[11].setTranslate(glm::vec3(0.0f, 0.f, 0.5f));
   mCubes[11].setScale(glm::vec3(0.25f, 0.5f, 0.5f));
   mCubes[11].setColor(tailColor);
   mCubes[11].translateHealthBar(glm::vec3(0.0f, 0.3f, 0.0f));
   mTail->add(&mCubes[11]);

   /// Fans are children of the tail
   const glm::vec4 fansColor(1.0f, 0.5f, 0.0f, 1.0f);
   // Left fan
   mCubes[12].setTranslate(glm::vec3(-0.4f, 0.f, 0.5f));
   mCubes[12].setRotate(glm::vec3(45.0f, 0.0f, 0.0f));
   mCubes[12].setScale(glm::vec3(1.0f, 0.1f, 0.5f));
   mCubes[12].setColor(fansColor);
   mCubes[12].translateHealthBar(glm::vec3(0.0f, 0.3f, 0.0f));
   mTail->add(&mCubes[12]);

   // Right fan
   mCubes[13].setTranslate(glm::vec3(0.4f, 0.f, 0.5f));
   mCubes[13].setRotate(glm::vec3(45.0f, 0.0f, 0.0f));
   mCubes[13].setScale(glm::vec3(1.0f, 0.1f, 0.5f));
   mCubes[13].setColor(fansColor);
   mCubes[13].translateHealthBar(glm::vec3(0.0f, 0.3f, 0.0f));
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
