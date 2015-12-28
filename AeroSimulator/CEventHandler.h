#ifndef AERO_SIMULATOR_EVENT_HANDLER_H
#define AERO_SIMULATOR_EVENT_HANDLER_H

namespace AeroSimulatorEngine
{
   class CAppEvent;

   class CEventHandler
   {
   public:
      CEventHandler();
      virtual ~CEventHandler();

      virtual void handleEvent(CAppEvent *pEvent) = 0;
   };
}

#endif // AERO_SIMULATOR_EVENT_HANDLER_H
