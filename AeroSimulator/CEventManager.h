#ifndef AERO_SIMULATOR_EVENT_MANAGER_H
#define AERO_SIMULATOR_EVENT_MANAGER_H

#include <unordered_map>
#include <memory>
#include "CAppEvent.h"

namespace AeroSimulatorEngine
{
   // This is a singleton
   class CEventManager
   {
   public:
      static CEventManager& getInstance()
      {
         static CEventManager instance;
         return instance;
      }

      ~CEventManager();

      void broadcastEvent(EventID eventId);
      void sendEventToHandler(EventID eventId, CEventHandler& handler);
      bool registerEvent(EventID eventId);
      void attachEvent(EventID eventId, CEventHandler& handler);
      void detachEvent(EventID eventId, CEventHandler& handler);

   private:
      CEventManager();
      // Disable the copy constructor and copy assignment operator for Singleton
      CEventManager(const CEventManager&) = delete;
      void operator=(const CEventManager&) = delete;

      std::unordered_map<EventID, std::shared_ptr<CAppEvent> > mEventMap; ///@todo: think whether shared ptr is OK, maybe unique would be OK too
   };
}

#endif // AERO_SIMULATOR_EVENT_MANAGER_H
