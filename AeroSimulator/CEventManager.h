#ifndef AERO_SIMULATOR_EVENT_MANAGER_H
#define AERO_SIMULATOR_EVENT_MANAGER_H

#include <unordered_map>
#include <memory>
#include "CAppEvent.h"

#define GEventManager CEventManager::getInstance()

namespace AeroSimulatorEngine
{
   enum eGeneralEvents { DEBUG_MODE_EVENT, DEPTHBUF_EVENT, UPDATE, UPDATE_RENDERABLE, LAST_GENERAL_EVENT };
   enum eCameraEvents { UPDATE_CAMERA = LAST_GENERAL_EVENT,
                        INCREASE_PITCH, INCREASE_PITCH_STOP,
                        DECREASE_PITCH, DECREASE_PITCH_STOP};

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
