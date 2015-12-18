#include "CParticleSystem.h"
#include "CAnimationBillBoard.h"
#include "./src/shaders/CShader.h"
#include "CLog.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

SParticle::SParticle()
   : mPosition()
   , mVelocity()
   , mCurrentTime(0.0f)
{
}

CParticleSystem::CParticleSystem()
   : CParentGameObject()
   , mParticle()
   , mBillboard()
   , mCurrentTime(0.0f)
   , mMaxLifeTime(5.0f)
   , mScaledTRMatrix()
{
}

CParticleSystem::~CParticleSystem()
{
}

bool CParticleSystem::emit(float deltaTime)
{
   return false;
}

void CParticleSystem::update(const float deltaTime)
{
   mBillboard->update(deltaTime);
}

void CParticleSystem::addParticle(std::shared_ptr<CShader>& pShader, std::shared_ptr<CShader>& pColorShader)
{
   if (pShader && pColorShader)
   {
      pShader->link();
      pColorShader->link();

      const float width = 1.0f;
      const float height = 1.0f;
      const char* filePath = "../AeroSimulator/res/coin.dds";
      float dx = 5.f;

      mBillboard.reset(new CAnimationBillBoard());

      if (mBillboard)
      {
         if (mBillboard->loadTexture(filePath))
         {
            CLog::getInstance().log("* Particle billboard loaded: ", filePath);
         }

         //mBillboard->setTranslate(glm::vec3(-10.f + count*dx, -5.0f, 0.0f));
         mBillboard->setBillboardHeight(width);
         mBillboard->setBillboardWidth(height);
         //mBillboard->calculateModelMatrix();
         mBillboard->setFrameSize(glm::vec2(1.0f / 10.0f, 1.0f));

         mBillboard->setShadersAndBuffers(pShader);
         const glm::vec4 bBoxColor = glm::vec4(0.f, 1.f, 0.5f, 1.0f);
         mBillboard->setBoundingBox(pColorShader, bBoxColor);

         std::vector<CCompositeGameObject*> tree;
         mBillboard->traverse(tree);

         for (auto * pTree : tree)
         {
            if (pTree)
            {
               add(pTree);
            }
         }

         add(mBillboard.get());

         buildModelMatrix(glm::mat4x4(1.0f)); // Bind children positions to the root
      }
   }
}

void CParticleSystem::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   CParentGameObject::buildModelMatrix(parentTRMatrix);

   // For the cube we need to calculate the model matrix
   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
   mModelMatrix = mParentTRMatrix * mScaledTRMatrix;
}

void CParticleSystem::updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt)
{
   CParentGameObject::updateTRMatrix(trMatrix, dt);

   // Don't forget to change the cached scaled TR matrix
   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
}

void CParticleSystem::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   CParentGameObject::updateModelMatrix(rootModelMatrix);

   // Model matrix of the cube
   mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;
}
