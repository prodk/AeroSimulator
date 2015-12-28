#ifndef AERO_SIMULATOR_CGAME_H
#define AERO_SIMULATOR_CGAME_H

#include "CTask.h"

namespace AeroSimulatorEngine
{
   class CGame : public CTask
   {
   public:
      CGame();
      virtual ~CGame();

      explicit CGame(ePriority prio);

      virtual bool start();
      virtual void update(CTask* pTask);
      virtual void stop();
   };
}

#endif // AERO_SIMULATOR_CGAME_H
