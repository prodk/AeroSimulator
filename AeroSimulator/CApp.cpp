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

   ///@todo: add to a separate method. Add skybox
   if (mSkyBox->loadTexture("../AeroSimulator/res/sky_1024.bmp"))
   {
      CLog::getInstance().log("* Skybox loaded ../AeroSimulator/res/sky_1024.bmp");
   }

   mSkyBox->scale(glm::vec3(120.f, 120.f, 120.0f));
   mTextureShader->link();
   mSkyBox->setShadersAndBuffers(mTextureShader);
   mRendererTask->addRenderable(mSkyBox.get());

   ///@todo: reconsider this system such that we should not keep in memory all this loading stuff
   // Add land
   if (mLand->loadTexture("../AeroSimulator/res/ground.bmp"))
   {
      CLog::getInstance().log("* Land loaded ../AeroSimulator/res/ground.bmp");
   }
   mLand->translate(glm::vec3(0.f, -10.f, 0.f));
   mLand->scale(glm::vec3(120.f, 0.f, 120.0f));
   mLand->setShadersAndBuffers(mTextureShader);
   mRendererTask->addRenderable(mLand.get());

   ///@todo: add to a separate method setupModels()
   mAirPlane->buildModel();
   std::vector<CCompositeGameObject*> tree; //Add cubes from the air plane
   mAirPlane->getTree(tree); ///@todo: remove the getTree()

   // Create all the shaders in mRendererTask and then add them to the model
   mSimpleShader->link();

   const std::size_t numOfCubes = tree.size();
   for (std::size_t count = 0; count < numOfCubes; ++count)
   {
      // Only leafs have valid geometry and a model matrix
      if (tree[count] /*&& tree[count]->isLeaf()*/)
      {
         tree[count]->setShadersAndBuffers(mSimpleShader);
         mRendererTask->addRenderable(tree[count]);
      }
   }

   // Set the root for the renderable composite
   mRendererTask->setRoot(mAirPlane->getRoot());

   // Add billboards
   if (mBillBoard->loadTexture("../AeroSimulator/res/ground.bmp"))
   {
      CLog::getInstance().log("* Billboard loaded ../AeroSimulator/res/ground.bmp");
   }
   mBillBoard->setTranslate(glm::vec3(0.f, 0.f, -2.f));
   mBillBoard->setRotate(glm::vec3(90.f, 0.f, 0.f));
   mBillBoard->setScale(glm::vec3(2.f, 1.f, 2.0f));
   mBillBoard->calculateModelMatrix();
   mBillBoard->setShadersAndBuffers(mTextureShader);
   mRendererTask->addRenderable(mBillBoard.get());

   mRendererTask->resetRenderContext();
}

