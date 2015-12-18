#include "CParticleSystem.h"
#include "CAnimationBillBoard.h"

using namespace AeroSimulatorEngine;

SParticle::SParticle()
   : mPosition()
   , mVelocity()
   , mCurrentTime(0.0f)
{
}

CParticleSystem::CParticleSystem()
   : CLeafGameObject()
   , mParticle()
   , mBillboard()
   , mCurrentTime(0.0f)
   , mMaxLifeTime(5.0f)
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
}

