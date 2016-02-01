#include "CGame.h"
#include "../AeroSimulator/CRenderer.h"
#include "../AeroSimulator/CApp.h"
#include "../AeroSimulator/CLog.h"
#include "../AeroSimulator/src/shaders/CShader.h"
#include "../AeroSimulator/src/shaders/CTextureShader.h"
#include "../AeroSimulator/src/shaders/CColorShader.h"
#include "../AeroSimulator/CGameObject.h"
#include "../AeroSimulator/CRenderableComponent.h"
#include "../AeroSimulator/CTimer.h"
#include "../AeroSimulator/CEventManager.h"
#include "../CCameraManager.h"

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
   : mShaders()
   , mGameObjects()
   , mFrameDt(0.0f)
{
}

CGame::~CGame()
{
   for (std::size_t count = 0; count < mShaders.size(); ++count)
   {
      mShaders[count].reset();
   }
   mShaders.clear();

   for (auto object : mGameObjects)
   {
      object.second.reset();
   }
   mGameObjects.clear();
}

CGame::CGame(ePriority prio)
   : CTask(prio)
   , mShaders(LAST_SHADER)
   , mGameObjects()
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

      addObjectsToRenderer();

      renderer()->resetRenderContext();
      result = true;
   }
   else
   {
      LOG("** CGame::start(): Render context is not setup **");
   }

   LOGGL("\n** CGame::start() ", result);
   return result;
}

void CGame::update(CTask * pTask)
{
   //LOG("CGame::update()");
   getTime(pTask);

   setObjectsTime();

   GEventManager.broadcastEvent(eGeneralEvents::UPDATE);

   ///@todo: change this method, probably to updateCameras()
   GCameraManager.updateCamera(0);
}

void CGame::stop()
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
   LOG("** CGame::setupScene() **");

   addLand();
}

void CGame::addLand()
{
   const char* filePath = "../AeroSimulator/res/land.dds";
   const int id = mGameObjects.size();

   ///@todo: change num of tiles to 10x10 when the camera is fixed
   tGoSharedPtr pObject(
      new CLand(id, eGameObjects::LAND, mShaders[eShaders::TEXTURE_SHADER], filePath, glm::vec2(2, 1)) );
   assert(pObject);

   tObjectPair newObject(id, pObject);

   if (mGameObjects.insert(newObject).second)
   {
      LOG("* CGame::addLand() success");
   }
   else
   {
      LOG("* CGame::addLand() failed to create land");
   }


   //mLand.reset(new CLand());
   //assert(mLand);

   //if (mLand)
   //{
      /*if (mLand->loadTexture("../AeroSimulator/res/land.dds"))
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

      renderer()->addGameObjectAndItsChildren(mLand.get());*/
   //}
}

void CGame::addObjectsToRenderer()
{
   for (auto object : mGameObjects)
   {
      if (object.second && canBeRendered(*object.second))
      {
         renderer()->addRenderable(&componentCast<CRenderableComponent>(*object.second)->getRenderable());
      }
   }
}

bool CGame::canBeRendered(CGameObject& object) const
{
   return object.hasComponent<CRenderableComponent>();
}

void CGame::getTime(CTask * pTask)
{
   if (pTask)
   {
      CTimer* pTimer = reinterpret_cast<CTimer*>(pTask);
      mFrameDt = pTimer->getDtFrame();
   }
}

void CGame::setObjectsTime()
{
   for (auto object : mGameObjects)
   {
      if (object.second)
      {
         object.second->setFrameDt(mFrameDt);
      }
   }
}
