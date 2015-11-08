// CApp.cpp - the application implementation

#include "CApp.h"
#include "CWin32Window.h"
#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CGeometry.h"
#include "CSimpleShader.h"
#include "CLog.h"
#include "CCube.h" //@todo: remove

#include "C3DModel.h"

#include <conio.h>
#include <cassert>
using namespace AeroSimulatorEngine;

CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(new CWin32Window(CTask::HIGHEST_PRIO))
   , mRendererTask(new CWin32Renderer(CTask::HIGH_PRIO))
   //, mCube(new CCube(0,
   //   glm::vec3(1.0f, 1.0f, 1.0f), // scale
   //   glm::vec3(0.0f, -30.0f, 0.0f), // rotate
   //   glm::vec3(0.0f, -2.0f, 0.0f))) ///@todo: replace
   , mCube(new CCube()) ///@todo: replace
   , mBodyCube(new CCube(mCube.get(), // parent  ///@todo: replace
                         glm::vec3(3.0f, 1.0f, 1.0f), // scale
                         glm::vec3(0.0f, 0.0f, 0.0f), // rotate
                         glm::vec3(0.0f, 1.0f, 0.0f)))// translate
   , mAirPlane(new C3DModel())
{
   assert(mAppWindowTask);
   assert(mRendererTask);
   ///@todo:asserts for game objects

   CLog::getInstance().log("* CApp created!");
}

CApp::~CApp()
{
   mCube.reset(); ///@todo: replace
   mBodyCube.reset();  ///@todo: replace

   mRendererTask.reset();
   mAppWindowTask.reset();

   CLog::getInstance().log("* CApp destroyed");
}

bool CApp::init(const char* name, unsigned int width, unsigned int height)
{
   // Create and init the app window and window's renderer
   bool result = false;
   if (mAppWindowTask && mAppWindowTask->create(name, width, height))
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

void CApp::setupRenderer()
{
   /// We need a valid RC to setup VBOs and shaders
   mRendererTask->setRenderContext();

   ///@todo: here setup game objects
   ///@todo: probably unite these into one method
   /*mCube->setupGeometry(); ///@todo: replace
   mCube->setupVBO(); ///@todo: replace

   mBodyCube->setupGeometry(); ///@todo: replace
   mBodyCube->setupVBO(); ///@todo: replace*/

   mAirPlane->buildModel();

   //mRendererTask->addRenderable(mCube.get()); ///@todo: replace
   //mRendererTask->addRenderable(mBodyCube.get()); ///@todo: replace


   //Add cubes from the air plane
   std::vector<CGameObject*> tree;
   mAirPlane->getTree(tree);

   const std::size_t numOfCubes = tree.size();
   for (std::size_t count = 0; count < numOfCubes; ++count)
   {
      tree[count]->setupGeometry();
      tree[count]->setupVBO();
      mRendererTask->addRenderable(tree[count]);
   }

   mRendererTask->setRenderContext();
}

