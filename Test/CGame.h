#ifndef AERO_SIMULATOR_CGAME_H
#define AERO_SIMULATOR_CGAME_H

#include "../CTask.h"
#include "../CEventHandler.h"
#include <memory>
#include <vector>
#include <map>

namespace AeroSimulatorEngine
{
   class CAppEvent;
   class CShader;
   class CGameObject;

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

      ///@todo: Add game events here

   public:
      // Shader ids
      enum eShaders {TEXTURE_SHADER, COLOR_SHADER, LAST_SHADER};
      enum eGameObjects {LAND};

      typedef std::pair<int, std::shared_ptr<CGameObject> > tObjectPair;
      typedef std::shared_ptr<CGameObject> tGoSharedPtr;

   private:
      void createShaders();
      void setupScene();
      void addLand();
      ///@todo: addObjectsToRenderer(): if the GO can be rendered, add its renderable to the renderer
      void addObjectsToRenderer();

      bool canBeRendered(CGameObject& object) const;

   private:
      std::vector<std::shared_ptr<CShader> > mShaders;
      std::map<int, std::shared_ptr<CGameObject> > mGameObjects;
   };
}

#endif // AERO_SIMULATOR_CGAME_H
