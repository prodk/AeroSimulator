#include "CTimer.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

float CTimer::getTimeFrame() const
{
   return 0.0f;
}

float CTimer::getTimeSim() const
{
   return 0.0f;
}

void CTimer::setSimMultiplier(const float simMultiplier)
{
   mSimMultiplier = simMultiplier;
}

bool CTimer::start()
{
   mTimeLastFrame = getCurrentTime();

   return false;
}

void CTimer::update()
{
   // Get the delta between the last frame and this
   double currentTime = getCurrentTime();
   mFrameDt = (currentTime - mTimeLastFrame) ;
   mTimeLastFrame = currentTime;
   mSimDt = mFrameDt * mSimMultiplier;
}

void CTimer::stop()
{
}

void CTimer::getTimeScale()
{
   // Get the performance timer frequency
   TimeUnits cntsPerSec = 0;
   bool perfExists = QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec) != 0;
   if (!perfExists)
   {
      CLog::getInstance().log("Performance timer does not exist!");
      return;
   }

   // Get a scaling factor which will convert the timer units to seconds.
   mTimeScale = 1.0 / (double)cntsPerSec;
}

double CTimer::getCurrentTime()
{
   TimeUnits time = 0;
   QueryPerformanceCounter((LARGE_INTEGER*)&time);
   double timeInSeconds = (double)(time)* mTimeScale;

   return timeInSeconds;
}
