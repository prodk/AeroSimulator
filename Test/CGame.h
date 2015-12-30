#ifndef AERO_SIMULATOR_CGAME_H
#define AERO_SIMULATOR_CGAME_H

#include "../CTask.h"
#include "../CEventHandler.h"
#include <memory>
#include <vector>

namespace AeroSimulatorEngine
{
   class CAppEvent;
   class CLand;
   class CShader;

   ///@todo: probably later do not make it a handler, move all event responses to the components:
   /// movement component, collision component
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

      // All events
      enum {DEBUG_MODE_EVENT, DEPTHBUF_EVENT}; ///@todo: probably move renderer-specific messages to other place

      // All shaders
      enum {TEXTURE_SHADER, COLOR_SHADER, LAST_SHADER};

   private:
      void createShaders();
      void setupScene();
      void addLand();

   private:
      std::unique_ptr<CLand> mLand;
      std::vector<std::shared_ptr<CShader> > mShaders; ///@todo: probably use unique_ptr
   };
}

#endif // AERO_SIMULATOR_CGAME_H
