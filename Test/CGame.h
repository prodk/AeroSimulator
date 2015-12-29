#ifndef AERO_SIMULATOR_CGAME_H
#define AERO_SIMULATOR_CGAME_H

#include "CTask.h"
#include "CEventHandler.h"

namespace AeroSimulatorEngine
{
   class CAppEvent;

   class CGame : public CTask, public CEventHandler
   {
   public:
      CGame();
      virtual ~CGame();

      explicit CGame(ePriority prio);

      // CTask part
      virtual bool start() override;
      virtual void update(CTask* pTask) override;
      virtual void stop() override;

      // CEventHandler part
      virtual void handleEvent(CAppEvent *pEvent) override;

      enum {DEBUG_MODE_EVENT, DEPTHBUF_EVENT}; ///@todo: probably move renderer-specific messages to other place
   };
}

#endif // AERO_SIMULATOR_CGAME_H
