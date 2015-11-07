// CApp.cpp - the application implementation

#include "CApp.h"
#include "CWin32Window.h"
#include "CWin32Renderer.h"
#include "CRenderable.h"
#include "CGeometry.h"
#include "CSimpleShader.h"
#include "CLog.h"

#include <conio.h>
using namespace AeroSimulatorEngine;

namespace
{
   GLfloat cubeData[] = {
       //front
      -1.0f, -1.0f,  1.0f,
      1.0f, 0.0f, 0.0f, //0 color
      1.0f, -1.0f,  1.0f,
      0.0f, 1.0f, 0.0f, //1 color
      1.0f,  1.0f,  1.0f,
      0.0f, 0.0f, 1.0f, //2 color
      -1.0f,  1.0f,  1.0f,
      0.0f, 0.0f, 0.0f, //3 color
      // back
      -1.0f, -1.0f, -1.0f,
      1.0f, 0.0f, 0.0f,//4 color
      1.0f, -1.0f, -1.0f,
      0.0f, 1.0f, 0.0f, //5 color
      1.0f,  1.0f, -1.0f,
      0.0f, 0.0f, 1.0f,//6 color
      -1.0f,  1.0f, -1.0f,
      1.0f, 0.0f, 1.0f //7 color
   };

   ///@todo: optimize the inidices
   ///@todo: this strip is not correct: it should be bottom->front->top, 2nd front is missing
   GLuint indices[] = {
      6, 5, 7, 4, // backd
      7, 4, 3, 0, // left
      3, 0, 2, 1, // front
      2, 1, 6, 5, // right
      5, 4, 1, 0, // bottom
      2, 3, 6, 7  // top
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

   CLog::getInstance().log("* CApp created!");
}

CApp::~CApp()
{
   mGeometry.reset();
   mShader.reset();

   mRenderable.reset();

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
   // Geometry setup
   CGeometry* pGeometry = mRenderable->getGeometry();

   pGeometry->setVertexBuffer(cubeData);
   const int numOfVertices = sizeof(cubeData) / sizeof(cubeData[0]);
   pGeometry->setNumOfVertices(numOfVertices);

   pGeometry->setIndexBuffer(indices);
   const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
   pGeometry->setNumOfIndices(numOfIndices);

   pGeometry->setNumOfElementsPerVertex(3); // 3 coordinates/color components per vertex
   pGeometry->setVertexStride(6); // stride of 6 for 3 coordinates and 3 colors

   mRendererTask->addRenderable(mRenderable.get());

   mRendererTask->generateVBOs();
}
