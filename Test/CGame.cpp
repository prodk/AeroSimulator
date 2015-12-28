#include "CGame.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

CGame::CGame()
{
}

CGame::~CGame()
{
}

CGame::CGame(ePriority prio)
   : CTask(prio)
{
}

bool CGame::start()
{
   bool result = true;
   return result;
}

void CGame::update(CTask * pTask)
{
   CLog::getInstance().log("CGame::update()");
}

void CGame::stop()
{
}
