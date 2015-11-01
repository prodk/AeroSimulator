//#pragma once

#ifndef AERO_SIMULATOR_CTASK_MANAGER_H
#define AERO_SIMULATOR_CTASK_MANAGER_H

#include "CTask.h"
#include <set>

// A comparator for the queue of tasks
struct CCompareTasks
{
   bool operator()(const CTask* lhs, const CTask* rhs)
   {
      bool result = false;
      if (lhs && rhs)
      {
         result = lhs->getPriority() > rhs->getPriority();
      }

      return result;
   }
};

class CTaskManager
{
public:
   CTaskManager();
   ~CTaskManager();

   bool addTask(CTask* pTask);
   bool removeTask(CTask* pTask);
   void killAllTasks();
   void execute();

   bool hasTasks() const { return !mTasks.empty(); }

   typedef std::set<CTask*, CCompareTasks> TTaskSet;
  // typedef std::set<CTask*, CCompareTasks>::iterator TTaskSetIter; // TODO: change later for the C++ 11 auto

private:
   TTaskSet mTasks;
};

#endif
