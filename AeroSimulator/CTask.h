//#pragma once

#ifndef AERO_SIMULATOR_CTASK_H
#define AERO_SIMULATOR_CTASK_H

class CTask
{
public:
   CTask();
   virtual ~CTask();

   virtual bool start() = 0;
   virtual bool update() = 0;
   virtual bool stop() = 0;

   void setCanKill(bool canKill);
   bool getCanKill() const { return mCanKill; }
   unsigned int getPriority() const { return mPriority; }

protected:
   bool mCanKill;
   unsigned int mPriority;
};

#endif

