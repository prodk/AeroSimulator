#include "CCubicAirPlane.h"
#include "../CTransform.h"
#include "../AeroSimulator/CTransformComponent.h"
#include "../AeroSimulator/CRenderable.h"
#include "../AeroSimulator/CFigure.h"
#include "CPropeller.h"
#include "../AeroSimulator/CUtils.h"
#include <algorithm>

using namespace AeroSimulatorEngine;

///@todo: probably add initial position to the ctr
CCubicAirPlane::CCubicAirPlane(const int id, const int type, std::shared_ptr<CShader> shader)
   : CGameObject(id, type)
   , mShader(shader)
   , mRollAngle(0.0f)
   , mPitch(0.0f)
   , mCurrentAction()
   , mCurrentState()
   , mPropeller()
   , mPropellerSpeed(mMinPropellerSpeed)
{
   std::vector<int> transformEvents(1, eGeneralEvents::UPDATE_TRANSFORM);
   (void)CGameObject::addTransformComponent(transformEvents, "* CCubicAirPlane: ");

   std::vector<int> moveEvents;
   moveEvents.push_back(eGeneralEvents::MOVE);
   moveEvents.push_back(eAirplaneEvents::LEAN_LEFT_START);
   moveEvents.push_back(eAirplaneEvents::LEAN_LEFT_STOP);
   moveEvents.push_back(eAirplaneEvents::LEAN_RIGHT_START);
   moveEvents.push_back(eAirplaneEvents::LEAN_RIGHT_STOP);
   moveEvents.push_back(eAirplaneEvents::GO_UP_START);
   moveEvents.push_back(eAirplaneEvents::GO_UP_STOP);
   moveEvents.push_back(eAirplaneEvents::GO_DOWN_START);
   moveEvents.push_back(eAirplaneEvents::GO_DOWN_STOP);

   (void)CGameObject::addMovementComponent(moveEvents, "* CCubicAirPlane: ");

   createParts();
}

CCubicAirPlane::~CCubicAirPlane()
{
}

void CCubicAirPlane::move()
{
   //LOG("CCubicAirPlane::move()");
   CTransformComponent* pTc = componentCast<CTransformComponent>(*this);
   if (pTc) {
      CTransform & rt = pTc->getTransform();
      const glm::vec3 prevTranslate = rt.getTranslate();
      const float v = 0.0f;
      const glm::vec3 newTranslate = prevTranslate + glm::vec3(0.0f, v*getFrameDt(), 0.0f);
      rt.setTranslate(newTranslate);

      lean(rt);
      changeHeight(rt);

      rt.setRotate(glm::vec3(mPitch, 0.0f, mRollAngle));
   }
}

void CCubicAirPlane::specificMove(const int moveType)
{
   switch (moveType) {
   case eAirplaneEvents::LEAN_LEFT_START:
      mCurrentState.reset(eCurrentState::LEAN_RIGHT);
      mCurrentState.set(eCurrentState::LEAN_LEFT);
      mCurrentAction.set(eCurrentAction::LEAN_ACT);
      break;

   case eAirplaneEvents::LEAN_LEFT_STOP:
      mCurrentAction.reset(eCurrentAction::LEAN_ACT);
      break;

   case eAirplaneEvents::LEAN_RIGHT_START:
      mCurrentState.reset(eCurrentState::LEAN_LEFT);
      mCurrentState.set(eCurrentState::LEAN_RIGHT);
      mCurrentAction.set(eCurrentAction::LEAN_ACT);
      break;

   case eAirplaneEvents::LEAN_RIGHT_STOP:
      mCurrentAction.reset(eCurrentAction::LEAN_ACT);
      break;

   case eAirplaneEvents::GO_UP_START:
      mCurrentState.reset(eCurrentState::GO_DOWN);
      mCurrentState.set(eCurrentState::GO_UP);
      mCurrentAction.set(eCurrentAction::GO_ACT);
      break;

   case eAirplaneEvents::GO_UP_STOP:
      mCurrentAction.reset(eCurrentAction::GO_ACT);
      break;

   case eAirplaneEvents::GO_DOWN_START:
      mCurrentState.reset(eCurrentState::GO_UP);
      mCurrentState.set(eCurrentState::GO_DOWN);
      mCurrentAction.set(eCurrentAction::GO_ACT);
      break;

   case eAirplaneEvents::GO_DOWN_STOP:
      mCurrentAction.reset(eCurrentAction::GO_ACT);
      break;
   }
}

void CCubicAirPlane::lean(CTransform & rt)
{
   const float maxRoll = 25.0f;
   const float leanSpeed = 0.5f;

   if (mCurrentState[eCurrentState::LEAN_LEFT])
   {
      if (mCurrentAction[eCurrentAction::LEAN_ACT])
      {
         mRollAngle = std::min<>(mRollAngle + leanSpeed, maxRoll);
      }
      else if (!mCurrentAction[eCurrentAction::LEAN_ACT])
      {
         mRollAngle = std::max<>(mRollAngle - leanSpeed, leanSpeed);
         if (std::fabs(mRollAngle) <= leanSpeed) {
            mCurrentState.reset(eCurrentState::LEAN_LEFT);
            mRollAngle = 0.0f;
         }
      }
   }

   if (mCurrentState[eCurrentState::LEAN_RIGHT])
   {
      if (mCurrentAction[eCurrentAction::LEAN_ACT])
      {
         mRollAngle = std::max<>(mRollAngle - leanSpeed, -maxRoll);
      }
      else if (!mCurrentAction[eCurrentAction::LEAN_ACT])
      {
         mRollAngle = std::min<>(mRollAngle + leanSpeed, leanSpeed);
         if (std::fabs(mRollAngle) <= leanSpeed) {
            mCurrentState.reset(eCurrentState::LEAN_RIGHT);
            mRollAngle = 0.0f;
         }
      }
   }
}

void CCubicAirPlane::changeHeight(CTransform & rt)
{
   const float maxPitch = 25.0f;
   const float leanSpeed = 0.5f;

   if (mCurrentState[eCurrentState::GO_UP])
   {
      const float propellerAccel = 1.0f;
      if (mCurrentAction[eCurrentAction::GO_ACT])
      {
         mPitch = std::min<>(mPitch + leanSpeed, maxPitch);
         mPropellerSpeed = std::min<>(mPropellerSpeed + propellerAccel, mMaxPropellerSpeed);
      }
      else if (!mCurrentAction[eCurrentAction::GO_ACT])
      {
         mPitch = std::max<>(mPitch - leanSpeed, leanSpeed);
         if (std::fabs(mPitch) <= leanSpeed) {
            mCurrentState.reset(eCurrentState::GO_UP);
            mPitch = 0.0f;
         }
         mPropellerSpeed = std::max<>(mPropellerSpeed - propellerAccel, mMinPropellerSpeed);
      }

      mPropeller->setRotationSpeed(mPropellerSpeed);
   }

   if (mCurrentState[eCurrentState::GO_DOWN])
   {
      if (mCurrentAction[eCurrentAction::GO_ACT])
      {
         mPitch = std::max<>(mPitch - leanSpeed, -maxPitch);
      }
      else if (!mCurrentAction[eCurrentAction::GO_ACT])
      {
         mPitch = std::min<>(mPitch + leanSpeed, leanSpeed);
         if (std::fabs(mPitch) <= leanSpeed) {
            mCurrentState.reset(eCurrentState::GO_DOWN);
            mPitch = 0.0f;
         }
      }
   }
}

void CCubicAirPlane::createParts()
{
   CTransformComponent* pTc = getComponent<CTransformComponent>();
   if (pTc) {
      CTransform & rootTr = pTc->getTransform();
      rootTr.setTranslate(glm::vec3(0.0f, 29.75f, 0.0f));

      CTransform transform;

      /// Cabine
      const std::size_t cabineId = mChildren.size();
      const glm::vec4 cabineColor(0.0f, 0.0f, 1.0f, 1.0f);
      transform.setTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
      transform.setScale(glm::vec3(0.5f, 0.5f, 0.5f));
      CUtils::addColorCube(transform, cabineColor, mType, mShader, mChildren, "* CCubicAirPlane ");

      /// Attach the first child to the root
      this->addChild(mChildren[cabineId]);

      const float cabHeight = -0.75f;
      /// Body
      const glm::vec4 bodyColor(0.0f, 1.0f, 1.0f, 1.0f);
      transform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
      /// Cube 0
      transform.setTranslate(glm::vec3(0.0f, cabHeight, 0.0f));
      CUtils::addColorCube(transform, bodyColor, mType, mShader, mChildren, "* CCubicAirPlane ");

      /// Cube 1
      transform.setTranslate(glm::vec3(0.0f, cabHeight, 1.0f));
      CUtils::addColorCube(transform, bodyColor, mType, mShader, mChildren, "* CCubicAirPlane ");

      /// Cube 2
      transform.setTranslate(glm::vec3(0.0f, cabHeight, 2.0f));
      CUtils::addColorCube(transform, bodyColor, mType, mShader, mChildren, "* CCubicAirPlane ");

      /// Cube 3
      transform.setTranslate(glm::vec3(0.0f, cabHeight, 3.0f));
      CUtils::addColorCube(transform, bodyColor, mType, mShader, mChildren, "* CCubicAirPlane ");

      /// Wings
      const glm::vec4 wingColor(1.0f, 0.0f, 1.0f, 1.0f);
      const float wingX = 1.8f;

      // Left wing
      transform.setTranslate(glm::vec3((-1.0f - 0.5f*(wingX - 1.0f)), cabHeight, 1.25f));
      transform.setScale(glm::vec3(wingX, 0.1f, 1.1f));
      CUtils::addColorCube(transform, wingColor, mType, mShader, mChildren, "* CCubicAirPlane ");

      // Right wing
      transform.setTranslate(glm::vec3((1.0f + 0.5f*(wingX - 1.0f)), cabHeight, 1.25f));
      transform.setScale(glm::vec3(wingX, 0.1f, 1.1f));
      CUtils::addColorCube(transform, wingColor, mType, mShader, mChildren, "* CCubicAirPlane ");

      for (std::size_t i = 1; i < mChildren.size(); ++i)
         mChildren[0]->addChild(mChildren[i]);

      // Propeller
      mPropeller.reset(new CPropeller(mChildren.size(), mType, mShader, mPropellerSpeed));
      if (nullptr != mPropeller) {
         mChildren.insert(std::pair<int, std::shared_ptr<CGameObject>>(mChildren.size(), mPropeller));
      }
      else {
         LOG("* CCubicAirPlane::addColorCube() pObject is NULL");
      }
      mChildren[1]->addChild(mChildren[mChildren.size()-1]); // Add the propeller to the correct cube
   }
}
