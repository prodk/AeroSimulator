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

      virtual bool start() override;
      virtual void update(CTask* pTask) override;
      virtual void stop() override;

      enum {DEBUG_MODE_EVENT, DEPTHBUF_EVENT}; ///@todo: probably move renderer-specific messages to other place
   };
}

#endif // AERO_SIMULATOR_CGAME_H
