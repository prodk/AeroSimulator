#ifndef AERO_SIMULATOR_CGAME_H
#define AERO_SIMULATOR_CGAME_H

#include "../AeroSimulator/CTask.h"

#include <memory>
#include <vector>
#include <map>

namespace AeroSimulatorEngine ///@todo: probably use another namespace
{
   class CAppEvent;
   class CShader;
   class CGameObject;

   class CGame : public CTask
   {
   public:
      CGame();
      virtual ~CGame();

      explicit CGame(ePriority prio);

      // CTask part
      virtual bool start() override;
      virtual void update(CTask* pTask) override;
      virtual void stop() override;

   public:
      // Shader ids
      enum eShaders {TEXTURE_SHADER, COLOR_SHADER, LAST_SHADER};
      enum eGameObjects {LAND, SKY};

      typedef std::pair<int, std::shared_ptr<CGameObject> > tObjectPair;
      typedef std::shared_ptr<CGameObject> tGoSharedPtr;

   private:
      void createShaders();
      void setupScene();
      void addLand();
      void addSky();

      void addCameras();

      void addObjectsToRenderer();

      bool canBeRendered(CGameObject& object) const;

      void getTime(CTask* pTask);

      void setObjectsTime();

      void addObject(const int id, tGoSharedPtr pObject, const char* msg);

   private:
      std::vector<std::shared_ptr<CShader> > mShaders;
      std::map<int, std::shared_ptr<CGameObject> > mGameObjects;
      float mFrameDt; // Time elapsed from the previous frame
   };
}

#endif // AERO_SIMULATOR_CGAME_H
