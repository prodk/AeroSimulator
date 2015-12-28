#include "CAppEvent.h"
#include "CEventHandler.h"

using namespace AeroSimulatorEngine;

CAppEvent::CAppEvent()
   : mEventListeners()
   , mId(0)
{
}

CAppEvent::~CAppEvent()
{
}

CAppEvent::CAppEvent(const EventID id)
   : mEventListeners()
   , mId(id)
{
}

void CAppEvent::broadcast()
{
   for (auto * pListener : mEventListeners)
   {
      if (pListener)
      {
         pListener->handleEvent(this);
      }
   }
}

void CAppEvent::sendToHandler(CEventHandler & handler)
{
   auto iter = mEventListeners.find(&handler);
   if (iter != mEventListeners.end())
   {
      handler.handleEvent(this);
   }
}

void CAppEvent::attachListener(CEventHandler & listener)
{
   mEventListeners.insert(&listener);
}

void CAppEvent::detachListener(CEventHandler & listener)
{
   auto iter = mEventListeners.find(&listener);
   if (iter != mEventListeners.end())
   {
      mEventListeners.erase(&listener);
   }
}
