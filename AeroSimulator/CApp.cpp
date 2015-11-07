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
   //float cubeData[] = {
   //   // Bottom plane
   //   -0.5f, -0.5f, 0.5f,  //0
   //   1.0f, 0.0f, 0.0f,    //0 color
   //   -0.5f, -0.5f, -0.5f, //1
   //   0.0f, 1.0f, 0.0f,    //1 color
   //   0.5f, -0.5f, -0.5f,  //2
   //   0.0f, 0.0f, 1.0f,    //2 color
   //   0.5f, -0.5f, 0.5f,   //3
   //   0.5f, 0.5f, 0.0f,    //3 color

   //   // Top plane
   //   -0.5f, 0.5f, -0.5f,  //4
   //   0.0f, 0.5f, 0.5f,    //4 color
   //   0.5f, 0.5f, -0.5f,   //5
   //   0.5f, 0.0f, 0.5f,    //5 color
   //   0.5f, 0.5f, 0.5f,    //6
   //   0.4f, 0.8f, 0.2f,    //6 color
   //   -0.5f, 0.5f, 0.5f,   //7
   //   0.8f, 0.2f, 0.4f     //7 color
   //};

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
   GLuint indices[] = {
      6, 5, 7, 4, // back
      7, 4, 3, 0, // left
      3, 0, 2, 1, // front
      2, 1, 6, 5, // right
      5, 4, 1, 0, // bottom
      2, 3, 6, 7 // top
   };

   //float cubeData[] = {
   //   // Bottom plane
   //   -0.5f, -0.5f, 0.5f,  //0
   //   -0.5f, -0.5f, -0.5f, //1
   //   0.5f, -0.5f, -0.5f,  //2
   //   0.5f, -0.5f, 0.5f,   //3

   //                        // Top plane
   //   -0.5f, 0.5f, -0.5f,  //4
   //   0.5f, 0.5f, -0.5f,   //5
   //   0.5f, 0.5f, 0.5f,    //6
   //   -0.5f, 0.5f, 0.5f,   //7
   //};

   //// 10 indices - a cube without 2 faces
   ///*GLuint indices[] = {
   //   6, 7, 3, 0, 2, 1, 5, 4, 6, 7
   //};*/

   // 27 indices - full cube
   //GLuint indices[] = {
   //   6, 7, 3, 0, 2, 1, 5, 4, 6, 7, // 10
   //   4, 1, 7, 0, 0, 1, 2, 2, 0, 3, 3, 2, 5, 3, 3, 6, 5 //17
   //};

   //GLushort indices[] = {
   //   // front
   //   0, 1, 2,
   //   2, 3, 0,
   //   // top
   //   //3, 2, 6,
   //   6, 7, 3,
   //   // back
   //   7, 6, 5,
   //   5, 4, 7,
   //   // bottom
   //   4, 5, 1,
   //   1, 0, 4,
   //   // left
   //   4, 0, 3,
   //   3, 7, 4,
   //   // right
   //   1, 5, 6,
   //   6, 2, 1
   //};

   //float cubeData[] = {
   //      -0.5f, -0.5f, 1.0f,  //0
   //      1.0f, 0.0f, 0.0f, //0 color
   //      0.5f, -0.5f, 1.0f, //1
   //      0.0f, 1.0f, 0.0f, //1 color
   //      0.5f, 0.5f, 1.0f,  //2
   //      0.0f, 0.0f, 1.0f, //2 color
   //      -0.5f, 0.5f, 1.0f,   //3
   //      0.0f, 1.0f, 1.0f, //3 color
   //};

   //GLuint indices[] = {
   //   //2, 3, 0
   //   3, 0, 2, 1
   //};
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

   mRendererTask.reset();
   mAppWindowTask.reset();

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

   pGeometry->setNumOfElementsPerVertex(3); // 3 coordinates/ color components per vertex, @todo: place to some variable to avoid magic numbers
   pGeometry->setVertexStride(3); // stride of 3 for colors

   mRendererTask->addRenderable(mRenderable.get());

   mRendererTask->generateVBOs();
}
