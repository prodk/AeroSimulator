#ifndef AERO_SIMULATOR_CPARENT_GAME_OBJECT_H
#define AERO_SIMULATOR_CPARENT_GAME_OBJECT_H

#include "CGameObject.h"

//#include <memory> /// is included in CRenderable.h

namespace AeroSimulatorEngine
{
   class CCube;

   // A node in the Composite which can have children.
   // Differences between a parent and a child node: 
   // i) parent contains children; ii) parent doesn't have valid geometry;
   // iii) parent doesn't have a valid model matrix;
   class CParentGameObject : public CGameObject
   {
   public:
      CParentGameObject();
      virtual ~CParentGameObject();

      CParentGameObject(const glm::vec3& scale,
                        const glm::vec3& rotate,
                        const glm::vec3& translate);

      ///Render-related overrides, do nothing
      //virtual void setupGeometry(std::shared_ptr<CGeometry>& pGeometry);
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      ///Composite-related overrides
      virtual void add(CGameObject* child);
      virtual void traverse(std::vector<CGameObject*>& tree);

      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);

      //virtual void updateMatrix(const glm::mat4& parentMatrix, const glm::mat4& dynamicMatrix);

      //void setDynamic();
      //bool isDynamic() const { return mIsDynamic; }

      //virtual glm::mat4 getChildTRMatrix(std::size_t childId) const;

   private:
      std::vector<CGameObject*> mChildren;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CPARENT_GAME_OBJECT_H
