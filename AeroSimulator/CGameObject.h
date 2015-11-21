#ifndef AERO_SIMULATOR_CGAME_OBJECT_H
#define AERO_SIMULATOR_CGAME_OBJECT_H

#include "CRenderable.h"
#include "glm/vec3.hpp"

#include <vector>

namespace AeroSimulatorEngine
{
   /// It is also the abstract base class in the Composite pattern.
   class CGameObject : public CRenderable
   {
   public:
      CGameObject();
      virtual ~CGameObject();

      CGameObject(const glm::vec3& scale,
                  const glm::vec3& rotate,
                  const glm::vec3& translate);

      /// Rendering-related methods
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader) = 0;

      void setScale(const glm::vec3& scale) { mScale = scale; }
      void setRotate(const glm::vec3& rotate) { mRotate = rotate; }
      void setTranslate(const glm::vec3& translate) { mTranslate = translate; }

      void calculateTRMatrix();

      void scale(const glm::vec3& scale);
      void translate(const glm::vec3& translate);

      glm::mat4 getTRMatrix() const;

      bool isLeaf() const { return mIsLeaf; }

   protected:
      ///@todo: probably move this to a separate CTransform class/component
      glm::vec3 mScale;       // Scale factors along the parent object axes
      glm::vec3 mRotate;      // Rotation angles in degrees around the parent axes
      glm::vec3 mTranslate;   // Translate along the parent axes

      glm::mat4 mTRMatrix;    // A local translate+rotate matrix
      glm::mat4 mParentTRMatrix; // Product of translate-rotate matrices of all the predecessors
      glm::mat4 mParentByLocalTRMatrix; // Avoid multiplying static matrices

      bool mIsLeaf;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CGAME_OBJECT_H
