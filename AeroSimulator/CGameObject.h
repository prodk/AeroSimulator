#ifndef AERO_SIMULATOR_CGAME_OBJECT_H
#define AERO_SIMULATOR_CGAME_OBJECT_H

#include "CRenderable.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <vector>

namespace AeroSimulatorEngine
{
   ///@todo: create a CCompositeGameObject, such that not all the game objects have to override the Composite pure vms
   ///It is also the abstract base class in the Composite pattern.
   class CGameObject : public CRenderable
   {
   public:
      CGameObject();
      virtual ~CGameObject();

      CGameObject(const glm::vec3& scale,
                  const glm::vec3& rotate,
                  const glm::vec3& translate);

      ///@todo: probably introduce one pure method setup() and call these 2 methods from it
      ///Rendering-related methods
      ///@todo: rename to setGeometry()
      //virtual void setGeometry(std::shared_ptr<CGeometry>& pGeometry) = 0;
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader) = 0;

      ///Composite-related methods to be overridden by children
      virtual void add(CGameObject* child) = 0;
      ///Saves elements to the provided array
      virtual void traverse(std::vector<CGameObject*>& tree) = 0;

      ///Rendering related composite methods
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix) = 0;
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix) = 0;
      virtual void updateModelMatrix(const glm::mat4x4 & modelMatrix) = 0;

      //virtual void updateMatrix(const glm::mat4& parentMatrix, const glm::mat4& dynamicMatrix);

      ///@todo: remove these methods
      /*virtual void setDynamic() { mIsDynamic = true; }
      virtual bool isDynamic() { return mIsDynamic; }
      virtual glm::mat4 getChildTRMatrix(std::size_t childId) const;*/

      // Transforms
      /*void resetModelMatrix(const glm::mat4& matrix);
      void resetTRMatrix(const glm::mat4& matrix);*/

      void setScale(const glm::vec3& scale);
      void setRotate(const glm::vec3& rotate);
      void setTranslate(const glm::vec3& translate);

      void calculateTRMatrix();

      void scale(const glm::vec3& scale);
      void translate(const glm::vec3& translate);

      glm::mat4 getTRMatrix() const;

      //void setParentTRMatrix(const glm::mat4& m) { mParentTRMatrix = m; }
      //glm::mat4 getParentTRMatrix() const { return mParentTRMatrix; }

      bool isLeaf() const { return mIsLeaf; }

   protected:
      ///@todo: probably move this to a separate CTransform class/component
      glm::vec3 mScale;   // Scale factors along the parent object axes
      glm::vec3 mRotate;  // Rotation angles in degrees around the parent axes
      glm::vec3 mTranslate;   // Translate along the parent axes

      glm::mat4 mTRMatrix; // A local translate+rotate matrix
                           ///@todo: probably remove this member as only the cached value is really used.
      glm::mat4 mParentTRMatrix; // Product of translate-rotate matrices of all the predecessors
      glm::mat4 mParentByLocalTRMatrix; // Avoid multiplying static matrices

      bool mIsLeaf;
      /*bool mIsDynamic;*/
   };

} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CGAME_OBJECT_H
