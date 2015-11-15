#ifndef AERO_SIMULATOR_CGAME_OBJECT_H
#define AERO_SIMULATOR_CGAME_OBJECT_H

#include "CRenderable.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <vector>

namespace AeroSimulatorEngine
{
   ///It is also the abstract base class in the Composite pattern.
   class CGameObject : public CRenderable
   {
   public:
      CGameObject();
      virtual ~CGameObject();

      CGameObject(const CGameObject* parent,
                  const glm::vec3& scale,
                  const glm::vec3& rotate,
                  const glm::vec3& translate);

      ///@todo: probably introduce one pure method setup() and call these 2 methods from it
      ///Rendering-related methods
      ///@todo: rename to setGeometry()
      virtual void setupGeometry(std::shared_ptr<CGeometry>& pGeometry) = 0;
      virtual void setupShadersAndBuffers() = 0;

      ///Composite-related methods to be overridden by children
      virtual void add(CGameObject* child) = 0;
      ///Saves elements to the provided array
      virtual void traverse(std::vector<CGameObject*>& tree) = 0;
      bool isLeaf() const { return mIsLeaf; }

      // Transforms
      void resetModelMatrix(const glm::mat4& matrix);
      void resetTRMatrix(const glm::mat4& matrix);
      glm::mat4 getTRMatrix() const;

      void scale(const glm::vec3& scale);
      void rotate(const glm::vec3& rotate);
      void translate(const glm::vec3& translate);

   protected:
      ///@todo: save a parent ptr/reference and make it possible to easily set another parent and change the position accordingly.
      //glm::mat4 mModelMatrix;  // Resulting model matrix for the object

      ///@todo: probably move this to a separate CTransform class/component
      glm::vec3 mScale;   // Scale factors along the parent object axes
      glm::vec3 mRotate;  // Rotation angles in degrees around the parent axes
      glm::vec3 mTranslate;   // Translate along the parent axes

      glm::mat4 mTRMatrix; // Translate+rotate matrix

      bool mIsLeaf;
   };

} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CGAME_OBJECT_H
