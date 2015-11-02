// CTaskManager.cpp - implementation of a task manager

#include "CTaskManager.h"
#include "CAppWindow.h"
using namespace AeroSimulatorEngine;

CTaskManager::CTaskManager()
   : mTasks()
{
}

CTaskManager::~CTaskManager()
{
}

bool CTaskManager::addTask(CTask * pTask)
{
   bool result = false;

   if (pTask && pTask->start())
   {
      result = true;
      mTasks.insert(pTask);
      ///@todo: add log here
   }

   return result;
}

bool CTaskManager::removeTask(CTask * pTask)
{
   bool result = false;
   if (pTask && (mTasks.find(pTask) != mTasks.end()))
   {
      result = true;
      pTask->setCanKill(result);
   }

   return result;
}

void CTaskManager::killAllTasks()
{
   for (auto* pTask : mTasks)
   {
      if (pTask)
      {
         pTask->setCanKill(true);
      }
   }
}

void CTaskManager::execute()
{
   while (!mTasks.empty())
   {
      if (CAppWindow::isClosing())
      {
         killAllTasks();
      }

      // Update all the tasks
      for (auto iter = std::begin(mTasks); iter != std::end(mTasks); ++iter)
      {
         CTask* pTask = (*iter);
         if (pTask && !pTask->getCanKill())
         {
            pTask->update();
         }
      }

      // Remove tasks that become obsolete
      for (auto iter = std::begin(mTasks); iter != std::end(mTasks);)
      {
         CTask* pTask = (*iter);
         ++iter;
         if (pTask && pTask->getCanKill())
         {
            pTask->stop();
            mTasks.erase(pTask);
            pTask = 0;
         }
      }
   }

   CAppWindow::resetIsClosing();
}

