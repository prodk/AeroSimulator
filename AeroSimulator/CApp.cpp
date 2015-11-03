// CApp.cpp - the application implementation

#include "CApp.h"
#include "CWin32Window.h"
#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CGeometry.h"
#include "CSimpleShader.h"

#include <conio.h>
using namespace AeroSimulatorEngine;

namespace
{
   float cubeData[] = {
      // Bottom plane
      -0.5f, -0.5f, 0.5f,  //0
      -0.5f, -0.5f, -0.5f, //1
      0.5f, -0.5f, -0.5f,  //2
      0.5f, -0.5f, 0.5f,   //3

      // Top plane
      -0.5f, 0.5f, -0.5f,  //4
      0.5f, 0.5f, -0.5f,   //5
      0.5f, 0.5f, 0.5f,    //6
      -0.5f, 0.5f, 0.5f,   //7
   };

   // 10 indices
   GLuint indices[] = {
      6, 7, 3, 0, 2, 1, 5, 4, 6, 7
   };
}

CApp::CApp()
   : mTaskManager()
   , mAppWindowTask(new CWin32Window(CTask::HIGHEST_PRIO))
   , mRendererTask(new CWin32Renderer(CTask::HIGH_PRIO))
   , mRenderable(new CRenderable())
   , mGeometry(new CGeometry())
   , mShader(new CSimpleShader())
{
   mRenderable->setGeometry(mGeometry.get());
   mRenderable->setShader(mShader.get());

   std::cout << "CApp created!" << std::endl;
}

CApp::~CApp()
{
   mGeometry.reset();
   mShader.reset();

   mRenderable.reset();

   mAppWindowTask.reset();
   mRendererTask.reset();

   std::cout << "CApp destroyed" << std::endl;
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

   std::cout << std::endl << "Press any key . . . " << std::endl;
   while (!_kbhit());
   return 0;
}

void CApp::setupRenderer()
{
   // Geometry setup
   CGeometry* pGeometry = mRenderable->getGeometry();

   pGeometry->setVertexBuffer(cubeData);
   const int numOfVertices = sizeof(cubeData) / sizeof(cubeData[0]);
   pGeometry->setNumOfVertices(numOfVertices);

   pGeometry->setIndexBuffer(indices);
   const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
   pGeometry->setNumOfIndices(numOfIndices);

   pGeometry->setNumOfElementsPerVertex(3); // 3 coordinates per vertex, @todo: place to some variable to avoid magic numbers
   pGeometry->setVertexStride(0);

   mRendererTask->addRenderable(mRenderable.get());

   mRendererTask->generateVBOs();
}
