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
#include "../AeroSimulator/CFigure.h"

#include "CLand.h"
#include "CSkyDom.h"
#include "CCubicAirPlane.h"

#include <cassert>

using namespace AeroSimulatorEngine;

namespace
{
   inline CRenderer * renderer()
   {
      return APP.getRenderer();
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

   GEventManager.broadcastEvent(eGeneralEvents::MOVE);
   GEventManager.broadcastEvent(eGeneralEvents::UPDATE_TRANSFORM);
   GEventManager.broadcastEvent(eCameraEvents::UPDATE_CAMERA);
   GEventManager.broadcastEvent(eGeneralEvents::UPDATE_RENDERABLE);
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
   addAirplane();
   addCameras();

   ///@todo: debug childrens structure: attach the camera to the cabine
   //mGameObjects[3]->addChild(mGameObjects[13]);
}

void CGame::addLand()
{
   const int id = mGameObjects.size();

   ///@todo: add later: read these params from a style file
   const std::string filePath = "../AeroSimulator/res/land.dds";
   const glm::vec3 landSize = glm::vec3(2000.f, 1.f, 2000.0f);

   SRenderableData data(mShaders[eShaders::TEXTURE_SHADER], 0, filePath);
   tGoSharedPtr pObject(
      new CLand(id, eGameObjects::LAND, data, glm::vec2(10, 10), landSize) );

   addObject(id, pObject, "* CGame::addLand() ");
}

void CGame::addSky()
{
   const int id = mGameObjects.size();

   ///@todo: add later: read these params from a style file
   const std::string filePath = "../AeroSimulator/res/sky.dds";
   const float radius = 300.0f;
   const std::size_t numOfCircles = 16;
   const std::size_t numOfSegments = 32;
   const float maxInclination = 0.5f * glm::pi<float>();
   const float maxAzimuth = 2.0f * glm::pi<float>();
   const SSphereParams params(radius, numOfCircles, numOfSegments, maxInclination, maxAzimuth);

   SRenderableData data(mShaders[eShaders::TEXTURE_SHADER], &params, filePath);
   tGoSharedPtr pObject(
      new CSkyDom(id, eGameObjects::SKY, data));

   addObject(id, pObject, "* CGame::addSky() ");
}

void CGame::addEnemies()
{
   const int id = mGameObjects.size();

   ///@todo: add later: read these params from a style file
   const std::size_t numOfCircles = 16;
   const std::size_t numOfSegments = 32;
   const std::string filePath = "../AeroSimulator/res/smile.dds";
   const SSphereParams params(2.0f, numOfCircles, numOfSegments, glm::pi<float>(), 2.0f * glm::pi<float>());

   SRenderableData data(mShaders[eShaders::TEXTURE_SHADER], &params, filePath, glm::vec4());

   CTransform transform;
   transform.setTranslate(glm::vec3(0.0f, 30.0f, -10.0f));
   transform.setRotate(glm::vec3(0.0f, 90.0f, 0.0f));
   tGoSharedPtr pObject(new CFigure(id, eGameObjects::AIRPLANE, CFigure::eFigure::SPHERE, data, transform));

   addObject(id, pObject, "* CGame::addEnemy ");
}

void CGame::addAirplane()
{
   const int id = mGameObjects.size();
   tGoSharedPtr pObject(
      new CCubicAirPlane(mGameObjects.size(), eGameObjects::AIRPLANE, mShaders[eShaders::COLOR_SHADER]));
   addObject(id, pObject, "* Game::addAirplane() ");
}

///@todo: remove if not used
//void CGame::addColorCube(const CTransform & transform, const glm::vec4 & color, const int objectType)
//{
//   const int id = mGameObjects.size();
//
//   SRenderableData data(mShaders[eShaders::COLOR_SHADER], 0, "", color);
//   tGoSharedPtr pObject(new CFigure(id, objectType, CFigure::eFigure::CUBE, data, transform));
//
//   addObject(id, pObject, "* CGame::addAirplane() CFigure cube ");
//}

void CGame::addCameras()
{
   // currently just 1 camera
   const int id = mGameObjects.size();
   const glm::vec3 translate(0.0f, 29.0f, -3.0f);
   const glm::vec3 rotate(0.0f, 180.0f, 0.0f);
   CTransform transform;
   transform.setTranslate(translate);
   transform.setRotate(rotate);

   ///@todo: add the type and the id of the game object to the constructor of the CCamera
   const SFrustum frustum(45.0f, 16.0f / 9.0f, 0.1f, 1100.0f);
   std::shared_ptr<CCamera> camera( new CCamera(id, CAMERA, transform, frustum));
   if (camera)
   {
      GCameraManager.addCamera(camera);
      LOG("* CGame: a camera has been just added to the camera manager.");

      addObject(id, camera, "* CGame::addCamera() ");
   }
}

void CGame::addObjectsToRenderer()
{
   for (auto object : mGameObjects)
   {
      if (nullptr != object.second)
      {
         object.second->addToRenderer(renderer());
      }
   }
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
