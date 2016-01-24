// CApp.cpp - the application implementation
#include "CApp.h"
#include "CWin32Window.h"
#include "CWin32Renderer.h"
//#include "CRenderable.h"
//#include "CGeometry.h"
//#include "CGameObject.h"
#include "CLog.h"
//#include "C3DModel.h"
//#include "../AeroSimulator/src/shaders/CTextureShader.h"
//#include "CSkyBox.h"
//#include "CBillBoard.h"
//#include "../AeroSimulator/src/shaders/CBillboardShader.h"
//#include "../AeroSimulator/src/shaders/CColorShader.h"
//#include "../AeroSimulator/src/shaders/CHealthbarShader.h"
//#include "CSphere.h"
//#include "../AeroSimulator/src/shaders/CColorLambertianShader.h"
#include "CTimer.h"
//#include "CAnimationBillBoard.h"
//#include "../src/shaders/CAnimationBillboardShader.h"
//#include "../src/shaders/CNormalMapSphereShader.h"
//#include "CParticleSystem.h"
//#include "CMissile.h"

#include <conio.h>
#include <cassert>
#include <random>
using namespace AeroSimulatorEngine;

CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(new CWin32Window(CTask::HIGHEST_PRIO_1))
   , mRendererTask(new CWin32Renderer(CTask::MEDIUM_PRIO))
   , mTimerTask(new CTimer(CTask::HIGHEST_PRIO_0))
   /*, mAirPlane(new C3DModel())
   , mTextureShader(new CTextureShader())
   , mBillboardShader(new CBillboardShader())
   , mColorShader(new CColorShader())
   , mHealthbarShader(new CHealthbarShader())
   , mColorLambertianShader(new CColorLambertianShader())
   , mAnimationBbShader(new CAnimationBillboardShader())
   , mSkyBox(new CSkyBox())
   , mSphere(new CSphere())
   , mBillBoards(40)
   , mStar(5)
   , mNormalMapSphereShader(new CNormalMapSphereShader())
   , mTurbineFire()
   , mTurbineSmoke()
   , mRightMissile()
   , mExplosion()*/
{
   assert(mAppWindowTask);
   assert(mRendererTask);
   assert(mTimerTask);
   /*assert(mAirPlane);
   assert(mTextureShader);
   assert(mSkyBox);
   assert(mBillboardShader);
   assert(mColorShader);
   assert(mHealthbarShader);
   assert(mColorLambertianShader);
   assert(mAnimationBbShader);
   assert(mSphere);
   assert(mNormalMapSphereShader);*/
   //assert(mTurbineFire);

   CLog::getInstance().log("\n*******************");
   CLog::getInstance().log("** CApp created! **\n");
}

CApp::~CApp()
{
   mRendererTask.reset();
   mAppWindowTask.reset();
   /*mAirPlane.reset();
   mTextureShader.reset();
   mSkyBox.reset();
   mBillboardShader.reset();
   mColorShader.reset();
   mSphere.reset();*/
   ///@todo: reset other ptrs here

   CLog::getInstance().log("\n** CApp destroyed **\n");
}

bool CApp::init(const char* name, unsigned int width, unsigned int height)
{
   // Create and init the app window and window's renderer
   bool result = false;
   if (mAppWindowTask && mAppWindowTask->create(name, width, height))
   {
      result = true;

      mRendererTask->init(*mAppWindowTask);

      ///@todo: move to CGame
      // Setup the renderable and set it to the renderer
      //setupScene();
   }

   return result;
}

void CApp::run()
{
   mTaskManager.addTask(mAppWindowTask.get());
   mTaskManager.addTask(mRendererTask.get());
   mTaskManager.addTask(mTimerTask.get());
   mTaskManager.setTaskToShare(mTimerTask.get());

   mTaskManager.execute();
}

int CApp::exit()
{
   mAppWindowTask = nullptr;
   mRendererTask = nullptr;

   CLog::getInstance().log("\nPress any key . . . ");
   while (!_kbhit());
   return 0;
}

void CApp::addTask(CTask *pTask)
{
   if (pTask)
   {
      mTaskManager.addTask(pTask);
   }
}

CRenderer* CApp::getRenderer() const
{
   return mRendererTask.get();
}

///@todo: move to CGameTask
//void CApp::setupScene()
//{
//   /// We need a valid RC to setup VBOs and shaders
//   mRendererTask->setRenderContext();
//
//   //addSkyBox();
//   //addLand();
//   //addAirplane();
//   //addClouds();
//   //addSphere();
//   //addStars();
//
//   mRendererTask->resetRenderContext();
//}

//void CApp::addSkyBox()
//{
//   if (mSkyBox->loadTexture("../AeroSimulator/res/sky.dds"))
//   {
//      CLog::getInstance().log("* Skybox loaded ../AeroSimulator/res/sky.dds");
//   }
//
//   mSkyBox->scale(glm::vec3(400.f, 200.f, 400.0f));
//
//   mTextureShader->link();
//   mSkyBox->setShadersAndBuffers(mTextureShader);
//   mRendererTask->addRenderable(mSkyBox.get());
//   mRendererTask->setSky(mSkyBox);
//}

//void CApp::addAirplane()
//{
//   mColorShader->link();
//   mHealthbarShader->link();
//   mAirPlane->setBillboardShader(mHealthbarShader);
//   mAirPlane->buildModel(mColorShader);
//
//   std::vector<CCompositeGameObject*> tree;
//   mAirPlane->getTree(tree);
//
//   const std::size_t numOfCubes = tree.size();
//   for (std::size_t count = 0; count < numOfCubes; ++count)
//   {
//      if (tree[count])
//      {
//         mRendererTask->addRenderable(tree[count]);
//      }
//   }
//
//   // Set the root for the renderable composite
//   mRendererTask->setAirplaneRoot(mAirPlane->getRoot());
//   mRendererTask->setAirplane(mAirPlane);
//
//   ///@todo: do not traverse the tree in addFire, smoke, missile, just traverse the airplane tree after these method calls
//   /// Jet fire
//   addFire();
//
//   /// Jet smoke
//   addSmoke();
//
//   addMissiles();
//}

//void CApp::addClouds()
//{
//   const float width = 20.0f;
//   const float height = 20.0f;
//   const float minDistance = 150.0f;
//   const float minDistanceY = -10.0f;
//   const float maxDistanceY = 0.3f*mSkyBox->getScale().y;
//   const char* filePath = "../AeroSimulator/res/cloud.dds";
//
//   mBillboardShader->link();
//
//   std::random_device rd;
//   std::mt19937 mtX(rd());
//   std::mt19937 mtY(rd());
//   std::mt19937 mtZ(rd());
//   std::uniform_real_distribution<float> distX(0.0f, 0.5f*(mSkyBox->getScale().x - width));
//   std::uniform_real_distribution<float> distY(minDistanceY, maxDistanceY);
//   std::uniform_real_distribution<float> sign(-1.f, 1.f);
//
//   for (std::size_t count = 0; count < mBillBoards.size(); ++count)
//   {
//      mBillBoards[count].reset(new CBillBoard());
//      if (mBillBoards[count])
//      {
//         if (mBillBoards[count]->loadTexture(filePath))
//         {
//            ///@todo: probably log this only to the log file
//            //CLog::getInstance().log("* Billboard loaded: ", filePath);
//         }
//
//         float x = distX(mtX)*(sign(mtX) > 0. ? 1.f : -1.f);
//         float z = distX(mtZ)*(sign(mtZ) > 0. ? 1.f : -1.f);
//         while (std::sqrt(x*x + z*z) < minDistance)
//         {
//            x = distX(mtX)*(sign(mtX) > 0. ? 1.f : -1.f);
//            z = distX(mtZ)*(sign(mtZ) > 0. ? 1.f : -1.f);
//         }
//
//         mBillBoards[count]->setTransparent(true);
//         mBillBoards[count]->setTranslate(glm::vec3(x, distY(mtY), z));
//         mBillBoards[count]->setPosition(glm::vec3(x, distY(mtY), z));
//
//         mBillBoards[count]->setBillboardHeight(width);
//         mBillBoards[count]->setBillboardWidth(height);
//         mBillBoards[count]->calculateModelMatrix();
//
//         mBillboardShader->link();
//         mBillBoards[count]->setShadersAndBuffers(mBillboardShader);
//         mRendererTask->addRenderable(mBillBoards[count].get());
//      }
//   }
//
//   mRendererTask->setClouds(mBillBoards);
//}

//void CApp::addSphere()
//{
//   const char* texturePath = "../AeroSimulator/res/smile.dds";
//   const char* animationPath = "../AeroSimulator/res/lightning.dds";
//
//   if (mSphere->loadTexture(texturePath))
//   {
//      CLog::getInstance().log("* Sphere texture loaded: ", texturePath);
//   }
//
//   const char* normalMapPath = "../AeroSimulator/res/bricks.dds";
//   if (mSphere->loadNormalMapTexture(normalMapPath))
//   {
//      CLog::getInstance().log("* Sphere normal map loaded: ", normalMapPath);
//   }
//
//   if (mSphere->loadAnimationTexture(animationPath))
//   {
//      CLog::getInstance().log("* Sphere animation texture loaded: ", animationPath);
//   }
//
//   //mColorLambertianShader->link();
//   mNormalMapSphereShader->link();
//
//   mSphere->createNonTexturedGeometry();
//   mSphere->setTranslate(glm::vec3(0.f, 0.f, -5.f));
//   mSphere->setScale(glm::vec3(1.f, 1.f, 1.f));
//   mSphere->calculateModelMatrix();
//   mSphere->setFrameSize(glm::vec2(1.0f / 8.0f, 1.0f));
//   //mSphere->setShadersAndBuffers(mColorLambertianShader);
//   mSphere->setShadersAndBuffers(mNormalMapSphereShader);
//   mSphere->addCustomObjects(mColorShader);
//
//   std::vector<CCompositeGameObject*> tree;
//   mSphere->traverse(tree);
//
//   for (auto * pTree : tree)
//   {
//      if (pTree)
//      {
//         mRendererTask->addRenderable(pTree);
//      }
//   }
//
//   mRendererTask->addRenderable(mSphere.get());
//   mRendererTask->setSphereRoot(mSphere.get());
//}

//void CApp::addStars()
//{
//   mAnimationBbShader->link();
//   const float width = 1.0f;
//   const float height = 1.0f;
//   const char* filePath = "../AeroSimulator/res/coin.dds";
//   float dx = 5.f;
//
//   for (std::size_t count = 0; count < mStar.size(); ++count)
//   {
//      mStar[count].reset(new CAnimationBillBoard());
//
//      if (mStar[count])
//      {
//         if (mStar[count]->loadTexture(filePath))
//         {
//            ///@todo: probably log this only to the log file
//            //CLog::getInstance().log("* Animation billboard loaded: ", filePath);
//         }
//
//         mStar[count]->setTranslate(glm::vec3(-10.f + count*dx, -5.0f, 0.0f));
//         mStar[count]->setBillboardHeight(width);
//         mStar[count]->setBillboardWidth(height);
//         mStar[count]->calculateModelMatrix();
//         mStar[count]->setFrameSize(glm::vec2(1.0f / 10.0f, 1.0f));
//         mStar[count]->setTransparent(true);
//
//         mStar[count]->setShadersAndBuffers(mAnimationBbShader);
//         mColorShader->link();
//         const glm::vec4 bBoxColor = glm::vec4(0.f, 1.f, 0.5f, 1.0f);
//         mStar[count]->setBoundingBox(mColorShader, bBoxColor);
//
//         std::vector<CCompositeGameObject*> tree;
//         mStar[count]->traverse(tree);
//
//         for (auto * pTree : tree)
//         {
//            if (pTree)
//            {
//               mRendererTask->addRenderable(pTree);
//            }
//         }
//
//         mRendererTask->addRenderable(mStar[count].get());
//         mRendererTask->setStars(mStar[count]);
//      }
//   } // End for
//}

//void CApp::addFire()
//{
//   mTurbineFire.reset(new CParticleSystem(1.0f, 4.0f, 16, glm::vec3(0.0f, 0.0f, 2.5f)));
//   if (mTurbineFire)
//   {
//      const char* filePath = "../AeroSimulator/res/fire_explosion.dds";
//      mTurbineFire->setTranslate(glm::vec3(0.0f, -0.35f, 1.75f));
//      mTurbineFire->addParticles(mAnimationBbShader, mColorShader, filePath, glm::vec2(4.0f, 4.0f), 0.5f, 0.5f);
//
//      std::vector<CCompositeGameObject*> tree;
//      mTurbineFire->traverse(tree);
//      for (auto * node : tree)
//      {
//         if (node)
//         {
//            mRendererTask->addRenderable(node);
//         }
//      }
//      mRendererTask->setTurbineFire(mTurbineFire);
//
//      // Add fire to the airplane
//      mAirPlane->getRoot()->add(mTurbineFire.get());
//   }
//}

//void CApp::addSmoke()
//{
//   mTurbineSmoke.reset(new CParticleSystem(1.0f, 1.0f, 8, glm::vec3(0.0f, 0.5f, 2.5f)));
//   if (mTurbineSmoke)
//   {
//      const char* filePath = "../AeroSimulator/res/smoke.dds";
//      mTurbineSmoke->setTranslate(glm::vec3(0.0f, -0.35f, 1.75f));
//      //mTurbineSmoke->setTranslate(glm::vec3(0.0f, 0.0f, 0.5f));
//      mTurbineSmoke->addParticles(mAnimationBbShader, mColorShader, filePath, glm::vec2(4.0f, 4.0f), 0.75f, 0.75f);
//
//      std::vector<CCompositeGameObject*> tree;
//      mTurbineSmoke->traverse(tree);
//      for (auto * node : tree)
//      {
//         if (node)
//         {
//            mRendererTask->addRenderable(node);
//         }
//      }
//      mRendererTask->setTurbineSmoke(mTurbineSmoke);
//
//      // Add fire to the airplane
//      mAirPlane->getRoot()->add(mTurbineSmoke.get());
//   }
//}

//void CApp::addMissiles()
//{
//   mRightMissile.reset(new CMissile());
//   if (mRightMissile)
//   {
//      const char* filePath = "../AeroSimulator/res/fire_explosion.dds";
//      mRightMissile->setTranslate(glm::vec3(2.0f, -1.0f, 1.0f));
//      mRightMissile->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
//
//      const glm::vec4 color(0.0f, 1.0f, 0.0f, 1.0f);
//      mRightMissile->setColor(color);
//      mColorShader->link();
//      mRightMissile->setShadersAndBuffers(mColorShader);
//
//      mRightMissile->addParticles(mAnimationBbShader, mColorShader, filePath, glm::vec2(4.0f, 4.0f), 0.5f, 0.5f);
//
//      //mRightMissile->buildModelMatrix(glm::mat4x4());
//      std::vector<CCompositeGameObject*> tree;
//      mRightMissile->traverse(tree);
//      for (auto * pTree : tree)
//      {
//         if (pTree)
//         {
//            mRendererTask->addRenderable(pTree);
//         }
//      }
//
//      ///Add missile to the renderer
//      mRendererTask->setRightMissile(mRightMissile);
//      mRendererTask->addRenderable(mRightMissile.get());
//
//      // Add missilee to the airplane
//      mAirPlane->getRoot()->add(mRightMissile.get());
//   }
//
//   // Add explosion
//   //mExplosion
//   mAnimationBbShader->link();
//   const float width = 1.0f;
//   const float height = 1.0f;
//   const char* filePath = "../AeroSimulator/res/fire_explosion.dds";
//
//   mExplosion.reset(new CAnimationBillBoard());
//
//   if (mExplosion)
//   {
//      if (mExplosion->loadTexture(filePath))
//      {
//         CLog::getInstance().log("* Explosion loaded: ", filePath);
//      }
//
//      mExplosion->setTranslate(glm::vec3(0.0f, 1.0f, 0.0f));
//      mExplosion->setBillboardHeight(width);
//      mExplosion->setBillboardWidth(height);
//      mExplosion->calculateModelMatrix();
//      mExplosion->setFrameSize(glm::vec2(1.0f / 4.0f, 1.0f / 4.0f));
//      //mExplosion->setFrameSize(glm::vec2(1.0f / 10.0f, 1.0f));
//      mExplosion->setTransparent(true);
//      mExplosion->setVisible(false); // Initially invisible
//
//      mExplosion->setShadersAndBuffers(mAnimationBbShader);
//      mColorShader->link();
//      const glm::vec4 bBoxColor = glm::vec4(0.f, 1.f, 0.5f, 1.0f);
//      mExplosion->setBoundingBox(mColorShader, bBoxColor);
//
//      std::vector<CCompositeGameObject*> tree;
//      mExplosion->traverse(tree);
//
//      for (auto * pTree : tree)
//      {
//         if (pTree)
//         {
//            mRendererTask->addRenderable(pTree);
//         }
//      }
//
//      mRendererTask->addRenderable(mExplosion.get());
//      mRendererTask->setExplosion(mExplosion);
//   }
//}
