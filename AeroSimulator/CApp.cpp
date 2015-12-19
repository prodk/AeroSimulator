// CApp.cpp - the application implementation
#include "CApp.h"
#include "CWin32Window.h"
#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CGeometry.h"
#include "../AeroSimulator/src/shaders/CSimpleShader.h"  ///@todo: probably remove this
#include "CGameObject.h"
#include "CLog.h"
#include "C3DModel.h"
#include "../AeroSimulator/src/shaders/CTextureShader.h"
#include "CSkyBox.h"
#include "CLand.h"
#include "CBillBoard.h"
#include "../AeroSimulator/src/shaders/CBillboardShader.h"
#include "../AeroSimulator/src/shaders/CColorShader.h"
#include "../AeroSimulator/src/shaders/CHealthbarShader.h"
#include "CSphere.h"
#include "../AeroSimulator/src/shaders/CColorLambertianShader.h"
#include "CTimer.h"
#include "CAnimationBillBoard.h"
#include "../src/shaders/CAnimationBillboardShader.h"
#include "../src/shaders/CNormalMapSphereShader.h"
#include "CParticleSystem.h"

#include <conio.h>
#include <cassert>
#include <random>
using namespace AeroSimulatorEngine;

CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(new CWin32Window(CTask::HIGHEST_PRIO_1))
   , mRendererTask(new CWin32Renderer(CTask::HIGH_PRIO))
   , mTimerTask(new CTimer(CTask::HIGHEST_PRIO_0))
   , mAirPlane(new C3DModel())
   , mSimpleShader (new CSimpleShader())  ///@todo: probably remove this
   , mTextureShader(new CTextureShader())
   , mBillboardShader(new CBillboardShader())
   , mColorShader(new CColorShader())
   //, mColorShader(new CColorLambertianShader())
   , mHealthbarShader(new CHealthbarShader())
   , mColorLambertianShader(new CColorLambertianShader())
   , mAnimationBbShader(new CAnimationBillboardShader())
   , mSkyBox(new CSkyBox())
   , mLand(new CLand())
   , mSphere(new CSphere())
   , mBillBoards(40)
   , mStar(5)
   , mNormalMapSphereShader(new CNormalMapSphereShader())
{
   assert(mAppWindowTask);
   assert(mRendererTask);
   assert(mTimerTask);
   assert(mAirPlane);
   assert(mSimpleShader);
   assert(mTextureShader);
   assert(mSkyBox);
   assert(mLand);
   assert(mBillboardShader);
   assert(mColorShader);
   assert(mHealthbarShader);
   assert(mColorLambertianShader);
   assert(mAnimationBbShader);
   assert(mSphere);
   assert(mNormalMapSphereShader);
   //assert(mTurbineFire);

   CLog::getInstance().log("* CApp created!");
}

CApp::~CApp()
{
   mRendererTask.reset();
   mAppWindowTask.reset();
   mAirPlane.reset();
   mSimpleShader.reset();
   mTextureShader.reset();
   mSkyBox.reset();
   mLand.reset();
   mBillboardShader.reset();
   mColorShader.reset();
   mSphere.reset();
   //mStar.reset();
   ///@todo: reset other ptrs here

   CLog::getInstance().log("* CApp destroyed");
}

bool CApp::init(const char* name, unsigned int width, unsigned int height)
{
   // Create and init the app window and window's renderer
   bool result = false;
   if (mAppWindowTask && mAppWindowTask->create(name, width, height, mRendererTask.get()))
   {
      result = true;

      mRendererTask->init(*mAppWindowTask);

      // Setup the renderable and set it to the renderer
      setupScene();
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

void CApp::setupScene()
{
   /// We need a valid RC to setup VBOs and shaders
   mRendererTask->setRenderContext();

   addSkyBox();
   addLand();
   addAirplane();
   //addClouds();
   //addSphere();
   //addStars();

   mRendererTask->resetRenderContext();
}

void CApp::addSkyBox()
{
   if (mSkyBox->loadTexture("../AeroSimulator/res/sky.dds"))
   {
      CLog::getInstance().log("* Skybox loaded ../AeroSimulator/res/sky.dds");
   }

   mSkyBox->scale(glm::vec3(400.f, 200.f, 400.0f));

   mTextureShader->link();
   mSkyBox->setShadersAndBuffers(mTextureShader);
   mRendererTask->addRenderable(mSkyBox.get());
   mRendererTask->setSky(mSkyBox);
}

void CApp::addLand()
{
   if (mLand->loadTexture("../AeroSimulator/res/land.dds"))
   {
      CLog::getInstance().log("* Land loaded ../AeroSimulator/res/land.dds");
   }
   const glm::vec3 landSize = glm::vec3(1000.f, 1.f, 1000.0f);
   mLand->setNumOfTiles(10, 10);
   mLand->setTranslate(glm::vec3(0.f, -14.f, 0.f));
   mLand->setScale(landSize);
   mLand->calculateModelMatrix();

   mTextureShader->link();
   mLand->setShadersAndBuffers(mTextureShader);
   mRendererTask->addRenderable(mLand.get());

   mColorShader->link();
   const glm::vec4 bBoxColor = glm::vec4(0.f, 1.f, 0.5f, 1.0f);
   mLand->setBoundingBox(mColorShader, landSize, bBoxColor);

   std::vector<CCompositeGameObject*> tree;
   mLand->traverse(tree);

   for (auto * pTree : tree)
   {
      if (pTree)
      {
         mRendererTask->addRenderable(pTree);
      }
   }

   mRendererTask->setLand(mLand);
}

void CApp::addAirplane()
{
   mColorShader->link();
   mHealthbarShader->link();
   mAirPlane->setBillboardShader(mHealthbarShader);
   mAirPlane->buildModel(mColorShader);

   std::vector<CCompositeGameObject*> tree;
   mAirPlane->getTree(tree);

   const std::size_t numOfCubes = tree.size();
   for (std::size_t count = 0; count < numOfCubes; ++count)
   {
      if (tree[count])
      {
         mRendererTask->addRenderable(tree[count]);
      }
   }

   // Set the root for the renderable composite
   mRendererTask->setAirplaneRoot(mAirPlane->getRoot());
   mRendererTask->setAirplane(mAirPlane);

   /// Add fire
   mTurbineFire.reset(new CParticleSystem(1.0f, 4.0f, 16, glm::vec3(0.0f, 0.0f, 2.5f)));
   if (mTurbineFire)
   {
      const char* filePath = "../AeroSimulator/res/fire_explosion.dds";
      mTurbineFire->setTranslate(glm::vec3(0.0f, -0.35f, 1.75f));
      mTurbineFire->addParticles(mAnimationBbShader, mColorShader, filePath, glm::vec2(4.0f, 4.0f));
      tree.clear();
      mTurbineFire->traverse(tree);
      for (auto * node : tree)
      {
         if (node)
         {
            mRendererTask->addRenderable(node);
         }
      }
      mRendererTask->setTurbineFire(mTurbineFire);

      // Add fire to the airplane
      mAirPlane->getRoot()->add(mTurbineFire.get());
   }

   /// Add smoke
}

void CApp::addClouds()
{
   const float width = 20.0f;
   const float height = 20.0f;
   const float minDistance = 150.0f;
   const float minDistanceY = -10.0f;
   const float maxDistanceY = 0.3f*mSkyBox->getScale().y;
   const char* filePath = "../AeroSimulator/res/cloud.dds";

   mBillboardShader->link();

   std::random_device rd;
   std::mt19937 mtX(rd());
   std::mt19937 mtY(rd());
   std::mt19937 mtZ(rd());
   std::uniform_real_distribution<float> distX(0.0f, 0.5f*(mSkyBox->getScale().x - width));
   std::uniform_real_distribution<float> distY(minDistanceY, maxDistanceY);
   std::uniform_real_distribution<float> sign(-1.f, 1.f);

   for (std::size_t count = 0; count < mBillBoards.size(); ++count)
   {
      mBillBoards[count].reset(new CBillBoard());
      if (mBillBoards[count])
      {
         if (mBillBoards[count]->loadTexture(filePath))
         {
            CLog::getInstance().log("* Billboard loaded: ", filePath);
         }

         float x = distX(mtX)*(sign(mtX) > 0. ? 1.f : -1.f);
         float z = distX(mtZ)*(sign(mtZ) > 0. ? 1.f : -1.f);
         while (std::sqrt(x*x + z*z) < minDistance)
         {
            x = distX(mtX)*(sign(mtX) > 0. ? 1.f : -1.f);
            z = distX(mtZ)*(sign(mtZ) > 0. ? 1.f : -1.f);
         }

         mBillBoards[count]->setTransparent(true);
         mBillBoards[count]->setTranslate(glm::vec3(x, distY(mtY), z));
         mBillBoards[count]->setPosition(glm::vec3(x, distY(mtY), z));

         mBillBoards[count]->setBillboardHeight(width);
         mBillBoards[count]->setBillboardWidth(height);
         mBillBoards[count]->calculateModelMatrix();

         mBillboardShader->link();
         mBillBoards[count]->setShadersAndBuffers(mBillboardShader);
         mRendererTask->addRenderable(mBillBoards[count].get());
      }
   }

   mRendererTask->setClouds(mBillBoards);
}

void CApp::addSphere()
{
   const char* texturePath = "../AeroSimulator/res/smile.dds";
   const char* animationPath = "../AeroSimulator/res/lightning.dds";

   if (mSphere->loadTexture(texturePath))
   {
      CLog::getInstance().log("* Sphere texture loaded: ", texturePath);
   }

   const char* normalMapPath = "../AeroSimulator/res/bricks.dds";
   if (mSphere->loadNormalMapTexture(normalMapPath))
   {
      CLog::getInstance().log("* Sphere normal map loaded: ", normalMapPath);
   }

   if (mSphere->loadAnimationTexture(animationPath))
   {
      CLog::getInstance().log("* Sphere animation texture loaded: ", animationPath);
   }

   //mColorLambertianShader->link();
   mNormalMapSphereShader->link();

   mSphere->createNonTexturedGeometry();
   mSphere->setTranslate(glm::vec3(0.f, 0.f, -5.f));
   mSphere->setScale(glm::vec3(1.f, 1.f, 1.f));
   mSphere->calculateModelMatrix();
   mSphere->setFrameSize(glm::vec2(1.0f / 8.0f, 1.0f));
   //mSphere->setShadersAndBuffers(mColorLambertianShader);
   mSphere->setShadersAndBuffers(mNormalMapSphereShader);
   mSphere->addCustomObjects(mColorShader);

   std::vector<CCompositeGameObject*> tree;
   mSphere->traverse(tree);

   for (auto * pTree : tree)
   {
      if (pTree)
      {
         mRendererTask->addRenderable(pTree);
      }
   }

   mRendererTask->addRenderable(mSphere.get());
   mRendererTask->setSphereRoot(mSphere.get());
}

void CApp::addStars()
{
   mAnimationBbShader->link();
   const float width = 1.0f;
   const float height = 1.0f;
   const char* filePath = "../AeroSimulator/res/coin.dds";
   float dx = 5.f;

   for (std::size_t count = 0; count < mStar.size(); ++count)
   {
      mStar[count].reset(new CAnimationBillBoard());

      if (mStar[count])
      {
         if (mStar[count]->loadTexture(filePath))
         {
            CLog::getInstance().log("* Animation billboard loaded: ", filePath);
         }

         mStar[count]->setTranslate(glm::vec3(-10.f + count*dx, -5.0f, 0.0f));
         mStar[count]->setBillboardHeight(width);
         mStar[count]->setBillboardWidth(height);
         mStar[count]->calculateModelMatrix();
         mStar[count]->setFrameSize(glm::vec2(1.0f / 10.0f, 1.0f));
         mStar[count]->setTransparent(true);

         mStar[count]->setShadersAndBuffers(mAnimationBbShader);
         mColorShader->link();
         const glm::vec4 bBoxColor = glm::vec4(0.f, 1.f, 0.5f, 1.0f);
         mStar[count]->setBoundingBox(mColorShader, bBoxColor);

         std::vector<CCompositeGameObject*> tree;
         mStar[count]->traverse(tree);

         for (auto * pTree : tree)
         {
            if (pTree)
            {
               mRendererTask->addRenderable(pTree);
            }
         }

         mRendererTask->addRenderable(mStar[count].get());
         mRendererTask->setStars(mStar[count]);
      }
   } // End for
}
