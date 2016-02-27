#ifndef AERO_SIMULATOR_CTIMER_SHADER_H
#define AERO_SIMULATOR_CTIMER_SHADER_H

#include <Windows.h>
#include <mmsystem.h>
#include "CTask.h"

namespace AeroSimulatorEngine
{
   class CTimer : public CTask
   {
   public:
      CTimer();
      virtual ~CTimer();
      explicit CTimer(ePriority prio);

      typedef __int64 TimeUnits;

      double getDtFrame() const;
      double getDtSim() const;
      void setSimMultiplier(const float simMultiplier);
      double getCurrentTime(); ///@todo: probably make virtual such that it can be overriden on different platforms

      int getFPS() const { return mFPS; }

      virtual bool start();
      virtual void update(CTask* pTask);
      virtual void stop();

   private:
      void getTimeScale();
      void calculateFPS();

   private:
      double mTimeScale;
      double mTimeLastFrame;
      double mFrameDt;
      double mSimDt;
      double mSimMultiplier;
      int mFPS;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CTIMER_SHADER_H
