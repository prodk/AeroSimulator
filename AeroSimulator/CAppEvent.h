#ifndef AERO_SIMULATOR_CAPP_EVENT_H
#define AERO_SIMULATOR_CAPP_EVENT_H

#include <set>

namespace AeroSimulatorEngine
{
   class CEventHandler;

   typedef unsigned int EventID; ///@todo: probably use an enum class here

   class CAppEvent // CEvent is reserved on Windows
   {
   public:
      CAppEvent();
      ~CAppEvent();

      explicit CAppEvent(const EventID id);

      void broadcast();
      void sendToHandler(CEventHandler& handler);
      void attachListener(CEventHandler& listener);
      void detachListener(CEventHandler& listener);
      EventID getId() const { return mId; }

   private:
      std::set<CEventHandler*> mEventListeners;
      EventID mId;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPP_EVENT_H
