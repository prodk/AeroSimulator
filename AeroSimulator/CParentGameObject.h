#ifndef AERO_SIMULATOR_CPARENT_GAME_OBJECT_H
#define AERO_SIMULATOR_CPARENT_GAME_OBJECT_H

#include "CCompositeGameObject.h"

namespace AeroSimulatorEngine
{
   class CCube;

   // A node in the Composite which can have children.
   // Differences between a parent and a child node: 
   // i) parent contains children; ii) parent may not have valid geometry;
   // iii) parent may not have a valid model matrix;
   class CParentGameObject : public CCompositeGameObject
   {
   public:
      CParentGameObject();
      virtual ~CParentGameObject();

      CParentGameObject(const glm::vec3& scale,
                        const glm::vec3& rotate,
                        const glm::vec3& translate);

      /// Render-related override, does nothing
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      ///Composite-related overrides
      virtual void add(CCompositeGameObject* child);
      virtual void traverse(std::vector<CCompositeGameObject*>& tree);

      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);

   protected:
      std::vector<CCompositeGameObject*> mChildren;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CPARENT_GAME_OBJECT_H
