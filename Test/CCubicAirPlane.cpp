#include "CCubicAirPlane.h"
#include "../CTransform.h"
#include "../AeroSimulator/CTransformComponent.h"
#include "../AeroSimulator/CRenderable.h"
#include "../AeroSimulator/CFigure.h"

using namespace AeroSimulatorEngine;

CCubicAirPlane::CCubicAirPlane(const int id, const int type, std::shared_ptr<CShader> shader)
   : CGameObject(id, type)
   , mShader(shader)
   , mCubes()
{
   std::vector<int> transformEvents(1, eGeneralEvents::UPDATE_TRANSFORM);
   (void)CGameObject::addTransformComponent(transformEvents, "* CCubicAirPlane: ");

   std::vector<int> moveEvents(1, eGeneralEvents::MOVE);
   (void)CGameObject::addMovementComponent(moveEvents, "* CCubicAirPlane: ");

   addCubes();
}

CCubicAirPlane::~CCubicAirPlane()
{
}

void CCubicAirPlane::move()
{
   //LOG("CCubicAirPlane::move()");
   CTransformComponent* pTc = componentCast<CTransformComponent>(*mCubes[0]);
   if (pTc) {
      CTransform & rt = pTc->getTransform();
      const glm::vec3 prevTranslate = rt.getTranslate();
      const float v = 0.2f;
      const glm::vec3 newTranslate = prevTranslate + glm::vec3(0.0f, v*getFrameDt(), 0.0f);
      rt.setTranslate(newTranslate);
   }
}

bool CCubicAirPlane::getChildren(std::map<int, std::shared_ptr<CGameObject>> & kids)
{
   kids = mCubes;
   return true;
}

void CCubicAirPlane::addCubes()
{
   CTransformComponent* pTc = getComponent<CTransformComponent>();
   if (pTc) {
      CTransform & rootTransform = pTc->getTransform();

      ///Cabine
      const std::size_t cabineId = mCubes.size();
      const glm::vec4 cabineColor(0.0f, 0.0f, 1.0f, 1.0f);
      rootTransform.setTranslate(glm::vec3(0.0f, 29.75f, 0.0f));
      rootTransform.setScale(glm::vec3(0.5f, 0.5f, 0.5f));
      addColorCube(rootTransform, cabineColor, mType);

      //this->addChild(mCubes[cabineId]);

      /// Body
      CTransform transform;
      const glm::vec4 bodyColor(0.0f, 1.0f, 1.0f, 1.0f);
      transform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
      /// Cube 0
      transform.setTranslate(glm::vec3(0.0f, -1.0f, 0.0f));
      addColorCube(transform, bodyColor, mType);

      /// Cube 1
      transform.setTranslate(glm::vec3(0.0f, -1.0f, 1.0f));
      addColorCube(transform, bodyColor, mType);

      /// Cube 2
      transform.setTranslate(glm::vec3(0.0f, -1.0f, 2.0f));
      addColorCube(transform, bodyColor, mType);

      /// Cube 3
      transform.setTranslate(glm::vec3(0.0f, -1.0f, 3.0f));
      addColorCube(transform, bodyColor, mType);

      /// Wings
      const glm::vec4 wingColor(1.0f, 0.0f, 1.0f, 1.0f);
      const float wingX = 1.8f;

      // Left wing
      transform.setTranslate(glm::vec3((-1.0f - 0.5f*(wingX - 1.0f)), -1.0f, 1.25f));
      transform.setScale(glm::vec3(wingX, 0.1f, 1.1f));
      addColorCube(transform, wingColor, mType);

      // Right wing
      transform.setTranslate(glm::vec3((1.0f + 0.5f*(wingX - 1.0f)), -1.0f, 1.25f));
      transform.setScale(glm::vec3(wingX, 0.1f, 1.1f));
      addColorCube(transform, wingColor, mType);

      // Propeller
      const glm::vec4 baseColor(1.0f, 0.5f, 0.0f, 1.0f);
      const glm::vec4 paddleColor(1.0f, 1.0f, 0.0f, 1.0f);

      // Base
      transform.setTranslate(glm::vec3(0.0f, -1.0f, -0.75f));
      transform.setScale(glm::vec3(0.2f, 0.25f, 0.5f));
      addColorCube(transform, baseColor, mType);

      // Paddle 1
      transform.setTranslate(glm::vec3(0.0f, -1.0f, -1.0f));
      transform.setScale(glm::vec3(0.1f, 1.5f, 0.1f));
      addColorCube(transform, paddleColor, mType);

      // Paddle 2
      transform.setTranslate(glm::vec3(0.0f, -1.0f, -1.0f));
      transform.setScale(glm::vec3(0.1f, 1.5f, 0.1f));
      addColorCube(transform, paddleColor, mType);

      for (int i = 1; i < mCubes.size(); ++i)
         mCubes[0]->addChild(mCubes[i]);
   }
}

void CCubicAirPlane::addColorCube(const CTransform & transform, const glm::vec4 & color, const int objectType)
{
   const int id = mCubes.size();

   SRenderableData data(mShader, 0, "", color);
   tGoSharedPtr pObject(new CFigure(id, objectType, CFigure::eFigure::CUBE, data, transform));
   if (nullptr != pObject) {
      mCubes.insert(std::pair<int, std::shared_ptr<CGameObject>>(id, pObject));
   }
   else {
      LOG("* CCubicAirPlane::addColorCube() pObject is NULL");
   }
}
