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

      typedef __int64 TimeUnits;

      float getTimeFrame() const;
      float getTimeSim() const;
      void setSimMultiplier(const float simMultiplier);
      double getCurrentTime(); ///@todo: probably make virtual such that it can be overriden on different platforms

      virtual bool start();
      virtual void update();
      virtual void stop();

   private:
      void getTimeScale();

   private:
      double mTimeScale;
      double mTimeLastFrame;
      double mFrameDt;
      double mSimDt;
      double mSimMultiplier;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CTIMER_SHADER_H
