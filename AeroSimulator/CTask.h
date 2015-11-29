//CTask - an abstrac class for a task

#ifndef AERO_SIMULATOR_CTASK_H
#define AERO_SIMULATOR_CTASK_H

namespace AeroSimulatorEngine
{
   class CTask
   {
   public:
      enum ePriority { HIGHEST_PRIO_0, HIGHEST_PRIO_1, HIGH_PRIO };

      CTask();
      explicit CTask(ePriority prio) : mCanKill(false), mPriority(prio) {}
      virtual ~CTask();

      virtual bool start() = 0;
      virtual void update(CTask* pTask) = 0;
      virtual void stop() = 0;

      void setCanKill(bool canKill);
      bool getCanKill() const { return mCanKill; }
      ePriority getPriority() const { return mPriority; }

   protected:
      bool mCanKill;
      enum ePriority mPriority;
   };

} // namespace AeroSimulatorEngine

#endif

