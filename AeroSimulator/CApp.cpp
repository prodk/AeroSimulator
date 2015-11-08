// CApp.cpp - the application implementation

#include "CApp.h"
#include "CWin32Window.h"
#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CGeometry.h"
#include "CSimpleShader.h"
#include "CLog.h"
#include "CCube.h"

#include <conio.h>
#include <cassert>
using namespace AeroSimulatorEngine;

CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(new CWin32Window(CTask::HIGHEST_PRIO))
   , mRendererTask(new CWin32Renderer(CTask::HIGH_PRIO))
   , mCube(new CCube())
   , mBodyCube(new CCube(glm::mat4(1.0f), // parent
      glm::vec3(3.0f, 1.0f, 1.0f), // scale
      glm::vec3(0.0f, 0.0f, 0.0f), // rotate
      glm::vec3(0.0f, 1.0f, 0.0f)))// translate
{
   assert(mAppWindowTask);
   assert(mRendererTask);
   ///@todo:asserts for game objects

   CLog::getInstance().log("* CApp created!");
}

CApp::~CApp()
{
   mCube.reset();
   mBodyCube.reset();

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
   mCube->setupGeometry();
   mCube->setupVBO();

   mBodyCube->setupGeometry();
   mBodyCube->setupVBO();

   mRendererTask->addRenderable(mCube.get());
   mRendererTask->addRenderable(mBodyCube.get());

   mRendererTask->setRenderContext();
}
