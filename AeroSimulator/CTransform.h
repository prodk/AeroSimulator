#ifndef AERO_SIMULATOR_CTRANSFORM_H
#define AERO_SIMULATOR_CTRANSFORM_H

#include "../AeroSimulator/include/glm/vec3.hpp"
#include "../AeroSimulator/include/glm/mat4x4.hpp"
#include "../AeroSimulator/include/glm/gtc/matrix_transform.hpp"

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
      glm::vec3 getRotate() const { return mRotate; }

      void setTranlate(const glm::vec3& translate);
      glm::vec3 getTranslate() const { return mTranslate; }

      glm::mat4 getModelMatrix() const { return mModelMatrix; }

      void updateModelMatrix();

      void updateTrMatrix();

      glm::mat4x4 getInverseRotateTranslate() const;

      void setTranslationFirst(bool first);

      bool operator!=(const CTransform& transform);
      CTransform& operator=(const CTransform& transform);

      enum {TRANSLATE_FIRST, ROTATE_FIRST};

   private:
      ///@todo: reconsider implementations of these methods as they seem to work vice versa
      void updateTranslateRotate(); // First rotate, then translate (read the name from right to left)
      void updateRotateTranslate(); // First translate, then rotate (read the name from right to left)

      void rotateTrMatrix();

   private:
      glm::vec3 mScale;       // Scale factors along the parent object axiss
      glm::vec3 mRotate;      // Rotation angles in degrees around the parent axis
      glm::vec3 mTranslate;   // Translate along the parent axis

      glm::mat4 mTRMatrix;    // A local translate+rotate matrix without scale
      glm::mat4 mModelMatrix; // Model matrix - brings the model to the world space
      int mTrType; // Defines in which sequence the TR matrices are multiplied, default: TRS
   };
}

#endif // AERO_SIMULATOR_CTRANSFORM_H
