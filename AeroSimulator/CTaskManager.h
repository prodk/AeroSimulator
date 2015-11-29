//CTaskManager.h - declaration of a task manager class

#ifndef AERO_SIMULATOR_CTASK_MANAGER_H
#define AERO_SIMULATOR_CTASK_MANAGER_H

#include "CTask.h"
#include <set>

namespace AeroSimulatorEngine
{
   // A comparator for the queue of tasks
   struct CCompareTasks
   {
      bool operator()(const CTask* lhs, const CTask* rhs) const
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

   private:
      TTaskSet mTasks;
   };

} // namespace AeroSimulatorEngine
#endif
