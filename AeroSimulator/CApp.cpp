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

#include <conio.h>
#include <cassert>
using namespace AeroSimulatorEngine;

CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(new CWin32Window(CTask::HIGHEST_PRIO))
   , mRendererTask(new CWin32Renderer(CTask::HIGH_PRIO))
   , mAirPlane(new C3DModel())
{
   assert(mAppWindowTask);
   assert(mRendererTask);

   ///asserts for game objects
   assert(mAirPlane);

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
   }

   return result;
}

void CApp::run()
{
   ///@todo: probably move this to the construction phase
   // Setup the renderable and set it to the renderer
   setupRenderer();

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

   mAirPlane->buildModel();

   //Add cubes from the air plane
   std::vector<CGameObject*> tree;
   mAirPlane->getTree(tree);

   const std::size_t numOfCubes = tree.size();
   for (std::size_t count = 0; count < numOfCubes; ++count)
   {
      ///@todo: remove setting geometry and shaders from here
      tree[count]->setupShadersAndBuffers();
      mRendererTask->addRenderable(tree[count]);
   }

   mRendererTask->setRenderContext();
}

