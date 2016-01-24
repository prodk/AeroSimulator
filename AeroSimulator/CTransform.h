#ifndef AERO_SIMULATOR_CTRANSFORM_H
#define AERO_SIMULATOR_CTRANSFORM_H

#include "../AeroSimulator/include/glm/vec3.hpp"
#include "../AeroSimulator/include/glm/mat4x4.hpp"

namespace AeroSimulatorEngine
{
   class CTransform
   {
   public:
      CTransform();
      ~CTransform();

      void setScale(const glm::vec3& scale);
      glm::vec3 getScale() const { return mScale; }

      void setRotate(const glm::vec3& rotate);
      glm::vec3 getRotate() const { return mRotate; }

      void setTranlate(const glm::vec3& translate);
      glm::vec3 getTranslate() const { return mTranslate; }


   private:
      glm::vec3 mScale;       // Scale factors along the parent object axiss
      glm::vec3 mRotate;      // Rotation angles in degrees around the parent axis
      glm::vec3 mTranslate;   // Translate along the parent axis

      //glm::mat4 mTRMatrix;  // A local translate+rotate matrix
      glm::mat4 mModelMatrix; // Model matrix - brings the model to the world space
   };
}

#endif // AERO_SIMULATOR_CTRANSFORM_H
