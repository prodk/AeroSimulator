#include "CTimer.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

CTimer::CTimer()
   : CTask()
   , mTimeScale(1.0)
   , mTimeLastFrame(1.0)
   , mFrameDt(0.0)
   , mSimDt(mFrameDt)
   , mSimMultiplier(1.0)
   , mFPS(0)
{
}

CTimer::CTimer(ePriority prio)
   :CTask(prio)
   , mTimeScale(1.0)
   , mTimeLastFrame(1.0)
   , mFrameDt(0.0)
   , mSimDt(mFrameDt)
   , mSimMultiplier(1.0)
   , mFPS(0)
{
}

CTimer::~CTimer()
{
}

float CTimer::getTimeFrame() const
{
   return mFrameDt;
}

float CTimer::getTimeSim() const
{
   return mSimDt;
}

void CTimer::setSimMultiplier(const float simMultiplier)
{
   mSimMultiplier = simMultiplier;
}

bool CTimer::start()
{
   getTimeScale();
   mTimeLastFrame = getCurrentTime();

   return true;
}

void CTimer::update(CTask* pTask)
{
   // Get the delta between the last frame and this
   double currentTime = getCurrentTime();
   mFrameDt = (currentTime - mTimeLastFrame) ;
   mTimeLastFrame = currentTime;
   mSimDt = mFrameDt * mSimMultiplier;

   calculateFPS();
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

void CTimer::calculateFPS()
{
   static int frameCnt = 0;
   static double timeElapsed = 0.0f;

   ++frameCnt;

   timeElapsed += mFrameDt;
   // Has one second passed?
   if (timeElapsed >= 1.0f)
   {
      mFPS = frameCnt;

      // Reset values
      frameCnt = 0;
      timeElapsed = 0.0f;
   }
}

double CTimer::getCurrentTime()
{
   TimeUnits time = 0;
   QueryPerformanceCounter((LARGE_INTEGER*)&time);
   double timeInSeconds = (double)(time)* mTimeScale;

   return timeInSeconds;
}
