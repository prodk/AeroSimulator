#ifndef AERO_SIMULATOR_CCAMERA_H
#define AERO_SIMULATOR_CCAMERA_H

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"


namespace AeroSimulatorEngine
{
   ///@todo: probably derive it from CGameObject
   class CCamera
   {
   public:
      CCamera();
      ~CCamera();

      void setProjectionMatrix(const glm::mat4& projection) { mProjectionMatrix = projection; }
      void setViewMatrix(const glm::mat4& view) { mViewMatrix = view; }
      void scale (const glm::vec3& scales);
      void translate(const glm::vec3& distance);
      void rotate(const glm::vec3& angles);
      void resetView();
      void update();

      glm::mat4 getProjectionMatrix() const { return mProjectionMatrix; }
      glm::mat4 getViewMatrix() const { return mViewMatrix; }

      glm::vec3 getRightVector() const;
      glm::vec3 getUpVector() const;

   private:
      glm::mat4 mScale;
      glm::mat4 mRotate;
      glm::mat4 mTranslate;
      glm::mat4 mViewMatrix;
      glm::mat4 mProjectionMatrix;
      glm::mat4 mNonScaledViewMatrix;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCAMERA_H
