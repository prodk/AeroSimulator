#ifndef AERO_SIMULATOR_CPARENT_GAME_OBJECT_H
#define AERO_SIMULATOR_CPARENT_GAME_OBJECT_H

#include "CGameObject.h"

#include <vector> ///@todo: probably use set in the future if searching elements is required
#include <memory>

namespace AeroSimulatorEngine
{
   class CCube;

   class CParentGameObject : public CGameObject
   {
   public:
      CParentGameObject();
      virtual ~CParentGameObject();

      CParentGameObject(const CGameObject* parent,
                        const glm::vec3& scale,
                        const glm::vec3& rotate,
                        const glm::vec3& translate);

      ///Render-related, do nothing
      virtual void setupGeometry();
      virtual void setupVBO();

      ///Composite-related
      virtual void add(CGameObject* child);
      virtual void traverse(std::vector<CGameObject*>& tree);

   private:
      std::vector<CGameObject*> mChildren;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CPARENT_GAME_OBJECT_H
