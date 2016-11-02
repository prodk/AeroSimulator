#include "CPropeller.h"
#include "../AeroSimulator/CRenderable.h"
#include "../AeroSimulator/CFigure.h"
#include "../CTransform.h"
#include "../AeroSimulator/CTransformComponent.h"
#include "../AeroSimulator/CUtils.h"

using namespace AeroSimulatorEngine;

CPropeller::CPropeller(const int id, const int type, std::shared_ptr<CShader> shader, float speed)
   : CGameObject(id, type)
   , mShader(shader)
   , mRotSpeed(speed)
{
   std::vector<int> transformEvents(1, eGeneralEvents::UPDATE_TRANSFORM);
   (void)CGameObject::addTransformComponent(transformEvents, "* CPropeller: ");

   std::vector<int> moveEvents(1, eGeneralEvents::MOVE);
   (void)CGameObject::addMovementComponent(moveEvents, "* CPropeller: ");

   /// Add the cubes
   CTransform transform;
   const glm::vec4 baseColor(1.0f, 0.5f, 0.0f, 1.0f);
   const glm::vec4 paddleColor(1.0f, 1.0f, 0.0f, 1.0f);

   // Base
   transform.setTranslate(glm::vec3(0.0f, 0.0f, -0.75f));
   transform.setScale(glm::vec3(0.2f, 0.25f, 0.5f));
   CUtils::addColorCube(transform, baseColor, mType, mShader, mChildren, "* Propeller ");

   /// Attach the first child to the root
   this->addChild(mChildren[0]);

   // Paddle 1
   const float length = 0.9f;
   const float halfLength = 0.5f*length;
   transform.setTranslate(glm::vec3(0.0f, halfLength, -0.3f));
   transform.setScale(glm::vec3(0.1f, length, 0.1f));
   CUtils::addColorCube(transform, paddleColor, mType, mShader, mChildren, "* Propeller ");

   // Paddle 2
   transform.setTranslate(glm::vec3(0.0f, -halfLength, -0.3f));
   transform.setScale(glm::vec3(0.1f, length, 0.1f));
   CUtils::addColorCube(transform, paddleColor, mType, mShader, mChildren, "* Propeller ");

   for (std::size_t i = 1; i < mChildren.size(); ++i)
      mChildren[0]->addChild(mChildren[i]);
}

CPropeller::~CPropeller()
{
}

void CPropeller::move()
{
   static float angle = 0.0f; ///@todo: make a member
   //LOG("CPropeller::move()");
   CTransformComponent* pTc = componentCast<CTransformComponent>(*this);
   if (pTc) {
      CTransform & rt = pTc->getTransform();
      rt.setRotate(glm::vec3(0.0f, 0.0f, angle));
      angle += mRotSpeed;
      angle = angle > 360.0f ? angle - 360.0f : angle;
   }
}
