// CApp.cpp - the application implementation
#include "CApp.h"
#include "CWin32Window.h"
#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CGeometry.h"
#include "CSimpleShader.h"
#include "CGameObject.h"
#include "CLog.h"
#include "C3DModel.h"
#include "CTextureShader.h"
#include "CSkyBox.h"
#include "CLand.h"
#include "CBillBoard.h"
#include "CBillboardShader.h"

#include <conio.h>
#include <cassert>
using namespace AeroSimulatorEngine;

CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(new CWin32Window(CTask::HIGHEST_PRIO))
   , mRendererTask(new CWin32Renderer(CTask::HIGH_PRIO))
   , mAirPlane(new C3DModel())
   , mSimpleShader (new CSimpleShader())
   , mTextureShader(new CTextureShader())
   , mSkyBox(new CSkyBox())
   , mLand(new CLand())
   , mBillBoard(new CBillBoard())
   , mBillboardShader(new CBillboardShader())
{
   assert(mAppWindowTask);
   assert(mRendererTask);
   assert(mAirPlane);
   assert(mSimpleShader);
   assert(mTextureShader);
   assert(mSkyBox);
   assert(mLand);
   assert(mBillBoard);

   CLog::getInstance().log("* CApp created!");
}

CApp::~CApp()
{
   mRendererTask.reset();
   mAppWindowTask.reset();

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
      setupRenderer();
   }

   return result;
}

void CApp::run()
{
   mTaskManager.addTask(mAppWindowTask.get());
   mTaskManager.addTask(mRendererTask.get());

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

///@todo: rename this method
void CApp::setupRenderer()
{
   /// We need a valid RC to setup VBOs and shaders
   mRendererTask->setRenderContext();

   addSkyBox();
   addLand();
   addAirplane();
   addBillboards();

   mRendererTask->resetRenderContext();
}

void CApp::addSkyBox()
{
   if (mSkyBox->loadTexture("../AeroSimulator/res/sky_1024.bmp"))
   {
      CLog::getInstance().log("* Skybox loaded ../AeroSimulator/res/sky_1024.bmp");
   }

   mSkyBox->scale(glm::vec3(35.f, 35.f, 35.0f));

   mTextureShader->link();
   mSkyBox->setShadersAndBuffers(mTextureShader);
   mRendererTask->addRenderable(mSkyBox.get());
}

void CApp::addLand()
{
   if (mLand->loadTexture("../AeroSimulator/res/ground.bmp"))
   {
      CLog::getInstance().log("* Land loaded ../AeroSimulator/res/ground.bmp");
   }
   mLand->setTranslate(glm::vec3(0.f, -17.f, 0.f));
   mLand->setScale(glm::vec3(35.f, 1.f, 35.0f));
   mLand->calculateModelMatrix();

   mTextureShader->link();
   mLand->setShadersAndBuffers(mTextureShader);
   mRendererTask->addRenderable(mLand.get());
}

void CApp::addAirplane()
{
   mAirPlane->buildModel();

   std::vector<CCompositeGameObject*> tree;
   mAirPlane->getTree(tree);

   // Every part of the tree uses the simple shader
   mSimpleShader->link();

   const std::size_t numOfCubes = tree.size();
   for (std::size_t count = 0; count < numOfCubes; ++count)
   {
      if (tree[count])
      {
         tree[count]->setShadersAndBuffers(mSimpleShader);
         mRendererTask->addRenderable(tree[count]);
      }
   }

   // Set the root for the renderable composite
   mRendererTask->setRoot(mAirPlane->getRoot());
}

void CApp::addBillboards()
{
   if (mBillBoard->loadTexture("../AeroSimulator/res/cloud_512_512.bmp"))
   {
      CLog::getInstance().log("* Billboard loaded ../AeroSimulator/res/cloud_512_512.bmp");
   }

   const float width = 3.0f;
   const float height = 3.0f;
   const float minDistance = 5.0f;
   const float maxDistance = 12.0f;

   ///@todo: generate 10 clouds with random positions here

   mBillBoard->setTranslate(glm::vec3(0.f, 0.f, -minDistance));
   mBillBoard->setBillboardHeight(width);
   mBillBoard->setBillboardWidth(height);
   mBillBoard->calculateModelMatrix();

   mBillboardShader->link();
   mBillBoard->setShadersAndBuffers(mBillboardShader);
   mRendererTask->addRenderable(mBillBoard.get());
}

