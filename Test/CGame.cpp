#include "CGame.h"
#include "CLog.h"
#include "CRenderer.h"
#include "CApp.h"
#include "CLand.h"

#include <cassert>

using namespace AeroSimulatorEngine;

namespace
{
   inline CRenderer * renderer()
   {
      return CApp::getInstance().getRenderer();
   }
}

CGame::CGame()
   : mLand()
{
}

CGame::~CGame()
{
   mLand.reset();
}

CGame::CGame(ePriority prio)
   : CTask(prio)
   , mLand()
{
}

bool CGame::start()
{
   bool result = false;

   // Setting up the scene, requires a valid rendering context
   if (renderer()->setRenderContext())
   {
      setupScene();

      renderer()->resetRenderContext();
      result = true;
   }
   else
   {
      CLog::getInstance().log("CGame::start(): Render context is not setup");
   }

   CLog::getInstance().logGL("CGame::start() ", result);
   return result;
}

void CGame::update(CTask * pTask)
{
   //CLog::getInstance().log("CGame::update()");
}

void CGame::stop()
{
}

void CGame::handleEvent(CAppEvent * pEvent)
{
}

void CGame::setupScene()
{
   addLand();
}

void CGame::addLand()
{
   mLand.reset(new CLand());
   assert(mLand);

   if (mLand)
   {
      if (mLand->loadTexture("../AeroSimulator/res/land.dds"))
      {
         CLog::getInstance().log("* Land loaded ../AeroSimulator/res/land.dds");
      }
      const glm::vec3 landSize = glm::vec3(1000.f, 1.f, 1000.0f);

      ///@todo: place the corresponding calls to the constructor and introduce additional parameters to the constructor
      mLand->setNumOfTiles(10, 10);
      mLand->setTranslate(glm::vec3(0.f, -14.f, 0.f));
      mLand->setScale(landSize);
      mLand->calculateModelMatrix();

      //mTextureShader->link(); ///@todo: no need to link shaders each time. Link them once after creation
      mLand->setShadersAndBuffers(mTextureShader);
      

      //mColorShader->link();
      const glm::vec4 bBoxColor = glm::vec4(0.f, 1.f, 0.5f, 1.0f);
      mLand->setBoundingBox(mColorShader, landSize, bBoxColor);

      renderer()->addGameObjectAndItsChildren(mLand.get());
   }
}
