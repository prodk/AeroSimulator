#include "CEventManager.h"

using namespace AeroSimulatorEngine;

CEventManager::CEventManager()
{
}

CEventManager::~CEventManager()
{
   for (auto iter : mEventMap)
   {
      if (iter.second)
      {
         iter.second.reset();
      }
   }
   mEventMap.clear();
}

void CEventManager::broadcastEvent(EventID eventId)
{
   auto eventIter = mEventMap.find(eventId);
   if (eventIter != mEventMap.end())
   {
      if (eventIter->second)
      {
         (eventIter->second)->broadcast();
      }
   }
}

void CEventManager::sendEventToHandler(EventID eventId, CEventHandler & handler)
{
   auto eventIter = mEventMap.find(eventId);
   if (eventIter != mEventMap.end())
   {
      if (eventIter->second)
      {
         (eventIter->second)->sendToHandler(handler);
      }
   }
}

bool CEventManager::registerEvent(EventID eventId)
{
   bool result = false;

   auto eventIter = mEventMap.find(eventId);
   if (eventIter == mEventMap.end())
   {
      std::shared_ptr<CAppEvent> newEvent(new CAppEvent(eventId));
      if (newEvent)
      {
         std::pair<EventID, std::shared_ptr<CAppEvent> > newPair(eventId, newEvent);
         auto iter = mEventMap.insert(newPair);
         result = iter.second;
      }
   }

   return result;
}

void CEventManager::attachEvent(EventID eventId, CEventHandler & handler)
{
   auto eventIter = mEventMap.find(eventId);
   if (eventIter != mEventMap.end())
   {
      if (eventIter->second)
      {
         (eventIter->second)->attachListener(handler);
      }
   }
}

void CEventManager::detachEvent(EventID eventId, CEventHandler & handler)
{
   auto eventIter = mEventMap.find(eventId);
   if (eventIter != mEventMap.end())
   {
      if (eventIter->second)
      {
         (eventIter->second)->detachListener(handler);
      }
   }
}
