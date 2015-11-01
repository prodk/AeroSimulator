//CTask - an abstrac class for a task

#ifndef AERO_SIMULATOR_CTASK_H
#define AERO_SIMULATOR_CTASK_H

namespace AeroSimulatorEngine
{
   class CTask
   {
   public:
      enum ePriority { HIGHEST_PRIO };

      CTask();
      explicit CTask(ePriority prio) : mCanKill(false), mPriority(prio) {}
      virtual ~CTask();

      virtual bool start() = 0;
      virtual bool update() = 0; // TODO: probably make it void
      virtual bool stop() = 0; // TODO: probably make it void

      void setCanKill(bool canKill);
      bool getCanKill() const { return mCanKill; }
      ePriority getPriority() const { return mPriority; }

   protected:
      bool mCanKill;
      enum ePriority mPriority;
   };

} // namespace AeroSimulatorEngine

#endif

