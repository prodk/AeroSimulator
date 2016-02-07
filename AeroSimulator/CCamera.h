#ifndef AERO_SIMULATOR_CCAMERA_H
#define AERO_SIMULATOR_CCAMERA_H

#include "CGameObject.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace AeroSimulatorEngine
{
   class CTransform;
   ///Important: all translations are specified as moving the scene relative to the camera in camera space.
   ///This allows us to use camera's model matrix as a view matrix without the need to make inversions.
   class CCamera : public CGameObject
   {
   public:
      CCamera(const CTransform& transform);
      ~CCamera();

      void setId(const int id) { mId = id; }
      int getId() const { return mId; }

      //void update();

      glm::mat4 getViewMatrix(); ///@todo: probably make it const later

   private:
      CTransform& getTransform();
      void registerEvents();

      /*void setProjectionMatrix(const glm::mat4& projection) { mProjectionMatrix = projection; }
      void setViewMatrix(const glm::mat4& view) { mViewMatrix = view; }
      void update();

      glm::mat4 getProjectionMatrix() const { return mProjectionMatrix; }
      glm::mat4 getViewMatrix() const { return mViewMatrix; }

      glm::vec3 getRightVector() const;
      glm::vec3 getUpVector() const;
      glm::vec3 getPositionWorldSpace() const;

      void setXzDirection(const glm::vec3& direction);*/

      //virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));

      // How it works:
      // i) take camera direction in the world space
      // ii) transform this direction to the camera space using the view matrix without translation
      // iii) move the scene along the direction in camera space;
      /*glm::mat3x3 getRotationMatrix() const;

      void translateLookAt(const glm::vec3& shift);
      glm::vec3 getLookAtPosition() const;
      void resetLookAt() { mLookAtMatrix = glm::mat4x4(1.0f); }*/

      //virtual void calculateTRMatrix();

   private:
      //glm::mat4 mViewMatrix;
      //glm::mat4 mProjectionMatrix;
      //glm::mat4 mNonScaledViewMatrix;
      //glm::mat4 mLookAtMatrix;  // Translates look at point relative to the parent
      int mId;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCAMERA_H
