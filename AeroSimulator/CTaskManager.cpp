#include "CTaskManager.h"

CTaskManager::CTaskManager()
   : mTasks()
{
}

CTaskManager::~CTaskManager()
{
}

void CTaskManager::execute()
{
}

bool CTaskManager::addTask(CTask * pTask)
{
   bool result = false;

   if (pTask && pTask->start())
   {
      result = true;
      mTasks.insert(pTask);
   }

   return result;
}

bool CTaskManager::removeTask(CTask * pTask)
{
   bool result = false;
   if (mTasks.find(pTask) != mTasks.end())
   {
      result = true;
      pTask->setCanKill(result);
   }

   return result;
}

