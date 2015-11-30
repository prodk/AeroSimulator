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
   , mHealthbarShader(new CHealthbarShader())
   , mColorLambertianShader(new CColorLambertianShader())
   , mSkyBox(new CSkyBox())
   , mLand(new CLand())
   , mSphere(new CSphere())
   , mBillBoards(20)
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
   assert(mSphere);

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
   addClouds();
   addSphere();

   mRendererTask->resetRenderContext();
}

void CApp::addSkyBox()
{
   if (mSkyBox->loadTexture("../AeroSimulator/res/sky.dds"))
   {
      CLog::getInstance().log("* Skybox loaded ../AeroSimulator/res/sky.dds");
   }

   mSkyBox->scale(glm::vec3(55.f, 55.f, 55.0f));

   mTextureShader->link();
   mSkyBox->setShadersAndBuffers(mTextureShader);
   mRendererTask->addRenderable(mSkyBox.get());
}

void CApp::addLand()
{
   if (mLand->loadTexture("../AeroSimulator/res/land.dds"))
   {
      CLog::getInstance().log("* Land loaded ../AeroSimulator/res/land.dds");
   }
   mLand->setTranslate(glm::vec3(0.f, -14.f, 0.f));
   mLand->setScale(glm::vec3(55.f, 1.f, 55.0f));
   mLand->calculateModelMatrix();

   mTextureShader->link();
   mLand->setShadersAndBuffers(mTextureShader);
   mRendererTask->addRenderable(mLand.get());
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
}

void CApp::addClouds()
{
   const float width = 3.0f;
   const float height = 3.0f;
   const float minDistance = 7.0f;
   const float maxDistance = 13.0f;
   const char* filePath = "../AeroSimulator/res/cloud.dds";

   mBillboardShader->link();

   std::random_device rd;
   std::mt19937 mt(rd());
   std::uniform_real_distribution<float> dist(minDistance, maxDistance);
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

         mBillBoards[count]->setTranslate(glm::vec3(dist(mt)*(sign(mt) > 0. ? 1.f : -1.f),
                                                    dist(mt)*(sign(mt) > 0. ? 1.f : -1.f),
                                                    dist(mt)*(sign(mt) > 0. ? 1.f : -1.f) ));
         mBillBoards[count]->setBillboardHeight(width);
         mBillBoards[count]->setBillboardWidth(height);
         mBillBoards[count]->calculateModelMatrix();

         mBillboardShader->link();
         mBillBoards[count]->setShadersAndBuffers(mBillboardShader);
         mRendererTask->addRenderable(mBillBoards[count].get());
      }
   }
}

void CApp::addSphere()
{
   mColorLambertianShader->link();

   mSphere->setTranslate(glm::vec3(0.f, 0.f, -5.f));
   mSphere->setScale(glm::vec3(1.f, 1.f, 1.f));
   mSphere->calculateModelMatrix();
   mSphere->setShadersAndBuffers(mColorLambertianShader);
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
