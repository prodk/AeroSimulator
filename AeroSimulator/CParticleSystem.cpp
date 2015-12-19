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
   , mIsAlive(false)
{
}

CParticleSystem::CParticleSystem()
   : CParentGameObject()
   , mParticle()
   , mBillboard()
   , mSystemTime(0.0f)
   , mMaxLifeTime(0.5f)
   , mScaledTRMatrix()
   , mEmitSpeed(16.0f)
   , mTimeToEmit(1.0f/mEmitSpeed)
   , mNumOfParticles(16)
   , mCurrentParticle(0)
   , mAliveParticles()
{
   mParticle.resize(mNumOfParticles);
   mBillboard.resize(mNumOfParticles);
}

CParticleSystem::~CParticleSystem()
{
}

bool CParticleSystem::emit(float deltaTime)
{
   bool result = false;

   mSystemTime += deltaTime;

   if (mSystemTime >= mTimeToEmit)// Emit a new particle if the old one is not alive
   {
      mCurrentParticle %= mNumOfParticles;
      if (!mParticle[mCurrentParticle].mIsAlive && mBillboard[mCurrentParticle])
      {
         mAliveParticles.insert(mCurrentParticle);
         // Set position to the one of the parent
         mParticle[mCurrentParticle].mPosition = getTranslate();  ///@todo: make it random within some narrow range
         mParticle[mCurrentParticle].mVelocity = glm::vec3(0.0f, 0.0f, 4.0f);
         mParticle[mCurrentParticle].mCurrentTime = 0.0f;
         mParticle[mCurrentParticle].mIsAlive = true;

         mBillboard[mCurrentParticle]->setTranslate(mParticle[mCurrentParticle].mPosition);

         calculateTRMatrix();
         buildModelMatrix(mParentTRMatrix);

         mBillboard[mCurrentParticle]->setVisible(true);

         result = true;

         mSystemTime = 0.0f;
         ++mCurrentParticle;
      }
   }

   return result;
}

void CParticleSystem::update(const float deltaTime)
{
   // Emit particles
   emit(deltaTime);

   // Update particles (move, animate)
   for (auto id : mAliveParticles)
   {
      if (mParticle[id].mIsAlive)
      {
         mParticle[id].mCurrentTime += deltaTime;

         if (mParticle[id].mCurrentTime >= mMaxLifeTime)
         {
            mParticle[id].mIsAlive = false;
            mParticle[id].mCurrentTime = 0.0f;
            mBillboard[id]->setVisible(false);
            //mAliveParticles.erase(id);
         }
         else
         {
            mParticle[id].mPosition += mParticle[id].mVelocity * deltaTime;

            mBillboard[id]->setTranslate(mParticle[id].mPosition);
            mBillboard[id]->calculateTRMatrix();

            mBillboard[id]->update(deltaTime);

            buildModelMatrix(mParentTRMatrix);
         }
      }
   }

   // Add to VBO the particles to show
}

void CParticleSystem::addParticles(std::shared_ptr<CShader>& pShader, std::shared_ptr<CShader>& pColorShader)
{
   if (pShader && pColorShader)
   {
      pShader->link();
      pColorShader->link();

      const float width = 0.5f;
      const float height = 0.5f;
      const char* filePath = "../AeroSimulator/res/fire_explosion.dds";
      float dx = 5.f;

      for (std::size_t id = 0; id < mNumOfParticles; ++id)
      {
         mBillboard[id].reset(new CAnimationBillBoard());

         if (mBillboard[id])
         {
            if (mBillboard[id]->loadTexture(filePath))
            {
               CLog::getInstance().log("* Particle billboard loaded: ", filePath);
            }

            mBillboard[id]->setBillboardHeight(width);
            mBillboard[id]->setBillboardWidth(height);
            //mBillboard->setFrameSize(glm::vec2(1.0f / 10.0f, 1.0f));
            mBillboard[id]->setFrameSize(glm::vec2(1.0f / 4.0f, 1.0f / 4.0f));
            mBillboard[id]->setAnimationSpeed(32.0f);
            mBillboard[id]->setTransparent(true);

            mBillboard[id]->setShadersAndBuffers(pShader);
            const glm::vec4 bBoxColor = glm::vec4(0.f, 1.f, 0.5f, 1.0f);
            mBillboard[id]->setBoundingBox(pColorShader, bBoxColor);
            //mBillboard->setVisible(false);
            std::vector<CCompositeGameObject*> tree;
            mBillboard[id]->traverse(tree);

            for (auto * pTree : tree)
            {
               if (pTree)
               {
                  add(pTree);
               }
            }

            add(mBillboard[id].get());
         }
      } // End for
      buildModelMatrix(glm::mat4x4(1.0f)); // Bind children positions to the root
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
