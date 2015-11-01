//#pragma once

#ifndef AERO_SIMULATOR_CTASK_H
#define AERO_SIMULATOR_CTASK_H

class CTask
{
public:
   enum ePriority { HIGHEST_PRIO };

   CTask();
   explicit CTask(ePriority prio) : mCanKill(false), mPriority(prio) {}
   virtual ~CTask();

   virtual bool start() = 0;
   virtual bool update() = 0;
   virtual bool stop() = 0;

   void setCanKill(bool canKill);
   bool getCanKill() const { return mCanKill; }
   ePriority getPriority() const { return mPriority; }

protected:
   bool mCanKill;
   enum ePriority mPriority;
};

#endif

