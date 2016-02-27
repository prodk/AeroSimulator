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
#include "../CCamera.h"
#include "../CTransform.h"
#include "../AeroSimulator/CUtils.h"

#include "CLand.h"
#include "CSkyDom.h"

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
   for (auto shader : mShaders)
   {
      shader.reset();
   }
   mShaders.clear();

   for (auto object : mGameObjects)
   {
      object.second.reset();
   }
   mGameObjects.clear();

   LOG("* CGame was destroyed");
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
   getTime(pTask);
   setObjectsTime();

   GEventManager.broadcastEvent(eGeneralEvents::UPDATE);
   GEventManager.broadcastEvent(eGeneralEvents::UPDATE_RENDERABLE);
   GEventManager.broadcastEvent(eCameraEvents::UPDATE_CAMERA);
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
   addSky();
   addEnemies();
   addCameras();
}

void CGame::addLand()
{
   const char* filePath = "../AeroSimulator/res/land.dds";
   const int id = mGameObjects.size();
   const glm::vec3 landSize = glm::vec3(2000.f, 1.f, 2000.0f);

   tGoSharedPtr pObject(
      new CLand(id, eGameObjects::LAND, mShaders[eShaders::TEXTURE_SHADER], filePath, glm::vec2(10, 10), landSize) );

   addObject(id, pObject, "* CGame::addLand() ");
}

void CGame::addSky()
{
   const std::string filePath = "../AeroSimulator/res/sky.dds";
   const int id = mGameObjects.size();

   const float radius = 300.0f;
   const std::size_t numOfCircles = 16;
   const std::size_t numOfSegments = 32;
   const float maxInclination = 0.5f * glm::pi<float>();
   const float maxAzimuth = 2.0f * glm::pi<float>();
   const SSphereParams params(radius, numOfCircles, numOfSegments, maxInclination, maxAzimuth);

   tGoSharedPtr pObject(
      new CSkyDom(id, eGameObjects::SKY, mShaders[eShaders::TEXTURE_SHADER], filePath, params));

   addObject(id, pObject, "* CGame::addSky() ");
}

void CGame::addEnemies()
{
   ///@todo: remove this debug
   const std::size_t numOfCircles = 16;
   const std::size_t numOfSegments = 32;

   const int idTest = mGameObjects.size();
   const std::string filePathTest = "../AeroSimulator/res/smile.dds";
   const SSphereParams paramsTest(10.0f, numOfCircles, numOfSegments, glm::pi<float>(), 2.0f * glm::pi<float>());

   tGoSharedPtr pObjectTest(
      new CSkyDom(idTest, eGameObjects::SKY, mShaders[eShaders::TEXTURE_SHADER], filePathTest, paramsTest));

   addObject(idTest, pObjectTest, "* CGame::addSky() test sphere");
   ///@todo: end
}

void CGame::addCameras()
{
   // currently just 1 camera
   const glm::vec3 translate(0.0f, 30.0f, 50.0f);
   const glm::vec3 rotate(0.0f, 0.0f, 0.0f);
   CTransform transform;
   transform.setTranlate(translate);
   transform.setRotate(rotate);

   //transform.setTranslationFirst(true);
   const SFrustum frustum(45.0f, 16.0f / 9.0f, 0.1f, 1100.0f);
   std::shared_ptr<CCamera> camera( new CCamera(transform, frustum));
   if (camera)
   {
      GCameraManager.addCamera(camera);
      LOG("* CGame: a camera has been just added to the camera manager.");

      addObject(mGameObjects.size(), camera, "* CGame::addCamera() ");
   }
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
      mFrameDt = static_cast<float>(pTimer->getDtFrame());
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

void CGame::addObject(const int id, tGoSharedPtr pObject, const char * msg)
{
   assert(pObject);

   tObjectPair newObject(id, pObject);

   if (mGameObjects.insert(newObject).second)
   {
      LOG(msg, "success\n");
   }
   else
   {
      LOG(msg, "failed\n");
   }
}
