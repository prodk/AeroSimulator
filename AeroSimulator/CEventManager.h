#ifndef AERO_SIMULATOR_EVENT_MANAGER_H
#define AERO_SIMULATOR_EVENT_MANAGER_H

#include <unordered_map>
#include <memory>
#include "CAppEvent.h"

#define GEventManager CEventManager::getInstance()

namespace AeroSimulatorEngine
{
   enum eGeneralEvents { DEBUG_MODE_EVENT, DEPTHBUF_EVENT, UPDATE_TRANSFORM, UPDATE_RENDERABLE, MOVE, LAST_GENERAL_EVENT };
   enum eCameraEvents { UPDATE_CAMERA = LAST_GENERAL_EVENT,
                        INCREASE_PITCH, INCREASE_PITCH_STOP,
                        DECREASE_PITCH, DECREASE_PITCH_STOP,
                        ROTATE_Y_CW, ROTATE_Y_CW_STOP,
                        ROTATE_Y_CCW, ROTATE_Y_CCW_STOP,
                        ROTATE_Z_CW, ROTATE_Z_CW_STOP,
                        ROTATE_Z_CCW, ROTATE_Z_CCW_STOP,
                        ZOOM_IN, ZOOM_IN_STOP, ZOOM_OUT, ZOOM_OUT_STOP,
                        MOVE_LEFT, MOVE_LEFT_STOP, MOVE_RIGHT, MOVE_RIGHT_STOP,
                        MOVE_UP, MOVE_UP_STOP, MOVE_DOWN, MOVE_DOWN_STOP,
                        LAST_CAMERA_EVENT};
   enum eAirplaneEvents {LEAN_LEFT_START = LAST_CAMERA_EVENT, LEAN_LEFT_STOP,
                         LEAN_RIGHT_START, LEAN_RIGHT_STOP,
                         GO_UP_START, GO_UP_STOP,
                         GO_DOWN_START, GO_DOWN_STOP};

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
