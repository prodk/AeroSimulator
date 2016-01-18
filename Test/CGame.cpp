#include "CGame.h"
#include "../CLog.h"
#include "../CRenderer.h"
#include "../CApp.h"
#include "../CLand.h"
#include "../AeroSimulator/src/shaders/CTextureShader.h"
#include "../AeroSimulator/src/shaders/CColorShader.h"

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
   , mShaders()
{
}

CGame::~CGame()
{
   mLand.reset();
}

CGame::CGame(ePriority prio)
   : CTask(prio)
   , mLand()
   , mShaders(LAST_SHADER)
{
}

bool CGame::start()
{
   bool result = false;

   // Setting up the scene, requires a valid rendering context
   if (renderer()->setRenderContext())
   {
      createShaders();
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

void CGame::createShaders()
{
   mShaders[TEXTURE_SHADER].reset(new CTextureShader());
   mShaders[COLOR_SHADER].reset(new CColorShader());

   for (std::size_t count = 0; count < mShaders.size(); ++count)
   {
      assert(mShaders[count]);
      mShaders[count]->link();
   }
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
      mLand->calculateModelMatrix(); ///@todo: place this somewhere such that no need to call it manually
      mLand->setShadersAndBuffers(mShaders[TEXTURE_SHADER]);

      renderer()->addGameObjectAndItsChildren(mLand.get());
   }
}
