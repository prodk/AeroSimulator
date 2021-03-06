#ifndef AERO_SIMULATOR_CTRANSFORM_H
#define AERO_SIMULATOR_CTRANSFORM_H

#include "../AeroSimulator/include/glm/vec3.hpp"
#include "../AeroSimulator/include/glm/mat4x4.hpp"
#include "../AeroSimulator/include/glm/gtc/matrix_transform.hpp"

#include <bitset>

namespace AeroSimulatorEngine
{
   class CTransform
   {
   public:
      CTransform();
      CTransform(const CTransform& transform);
      ~CTransform();

      void setScale(const glm::vec3& scale);
      glm::vec3 getScale() const { return mScale; }

      void setRotate(const glm::vec3& rotate);
      void setRotate(const glm::mat4& rotationMatrix);
      glm::mat4 getRotate() const { return mRotationMatrix; }

      void setTranslate(const glm::vec3& translate);
      glm::vec3 getTranslate() const { return mTranslate; }

      glm::mat4 getModelMatrix();

      void setTranslateRotateMatrix(const glm::mat4& m);
      //glm::mat4 getTranslateRotateMatrix() const { return mTRMatrix; }
      glm::mat4 getTranslateRotateMatrix();

      void updateTrMatrix();

      glm::mat4x4 getInverseRotateTranslate() const;

      void setTranslationFirst(bool first);

      bool operator!=(const CTransform& transform);
      CTransform& operator=(const CTransform& transform);

      enum {TRANSLATE_FIRST, ROTATE_FIRST};

   private:
      void updateTranslateRotate(); // First rotate, then translate (read the name from right to left)
      void updateRotateTranslate(); // First translate, then rotate (read the name from right to left)

   private:
      glm::vec3 mScale;       // Scale factors along the parent object axiss
      glm::vec3 mTranslate;   // Translate along the parent axis

      glm::mat4 mRotationMatrix;
      glm::mat4 mTRMatrix;    // A local translate+rotate matrix without scale, it is always local (no relation to parents), no scale

      int mTrType; // Defines in which sequence the TR matrices are multiplied, default: TRS

      ///@todo: remove eParentChanged because the transform should not know anything about parents. It works only locally.
      enum eTransformState
      {
         eTrChanged = 0, // local translate/rotate matrix has changed, means that TR matrix needs to be updated
         eParentChanged, // the parent matrix has changed, means that the model matrix needs to be updated
         eLastState
      };

      std::bitset<eLastState> mState;
   };
}

#endif // AERO_SIMULATOR_CTRANSFORM_H
