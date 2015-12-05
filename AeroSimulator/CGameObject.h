#ifndef AERO_SIMULATOR_CGAME_OBJECT_H
#define AERO_SIMULATOR_CGAME_OBJECT_H

#include "CRenderable.h"
#include "glm/vec3.hpp"

#include <vector>

namespace AeroSimulatorEngine
{
   /// Any game object can in principle be rendered
   class CGameObject : public CRenderable
   {
   public:
      CGameObject();
      virtual ~CGameObject();

      CGameObject(const glm::vec3& scale,
                  const glm::vec3& rotate,
                  const glm::vec3& translate);

      /// Rendering-related methods
      ///@todo: probably do not make it pure virtual and remove from CCube, CBillBoard and CLine
      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader) = 0;

      virtual void addCustomObjects(std::shared_ptr<CShader>& pShader);

      virtual void update(const float deltaTime);

      void setScale(const glm::vec3& scale) { mScale = scale; }
      void setRotate(const glm::vec3& rotate) { mRotate = rotate; }
      void setTranslate(const glm::vec3& translate) { mTranslate = translate; }

      glm::vec3 getTranslate() const { return mTranslate; }

      virtual void calculateTRMatrix();
      void calculateModelMatrix();

      void scale(const glm::vec3& scale);
      void translate(const glm::vec3& translate);

      glm::mat4 getTRMatrix() const;

      bool isLeaf() const { return mIsLeaf; }

      void resetParentTRMatrix() { mParentTRMatrix = glm::mat4x4(1.0f); }

   protected:
      ///@todo: probably move this to a separate CTransform class/component
      glm::vec3 mScale;       // Scale factors along the parent object axes
      glm::vec3 mRotate;      // Rotation angles in degrees around the parent axes
      glm::vec3 mTranslate;   // Translate along the parent axes

      glm::mat4 mTRMatrix;    // A local translate+rotate matrix
      glm::mat4 mParentTRMatrix; // Product of translate-rotate matrices of all the predecessors
      glm::mat4 mParentByLocalTRMatrix; // Avoid multiplying static matrices

      bool mIsLeaf;
      float mArbitraryAngle;
      glm::vec3 mArbitraryAxis;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CGAME_OBJECT_H
