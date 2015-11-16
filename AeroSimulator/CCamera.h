#ifndef AERO_SIMULATOR_CCAMERA_H
#define AERO_SIMULATOR_CCAMERA_H

#include "glm/mat4x4.hpp"

namespace AeroSimulatorEngine
{
   ///@todo: probably make it a singleton later
   ///@todo: probably derive it from CGameObject
   class CCamera
   {
   public:
      CCamera();
      ~CCamera();

      void setProjectionMatrix(const glm::mat4& projection) { mProjectionMatrix = projection; }
      void setViewMatrix(const glm::mat4& view) { mViewMatrix = view; }
      void translate(const glm::vec3& distance);
      void rotate(const glm::vec3& angles);
      void resetView();

      glm::mat4 getProjectionMatrix() const { return mProjectionMatrix; }
      glm::mat4 getViewMatrix() const { return mViewMatrix; }

   private:
      glm::mat4 mViewMatrix;
      glm::mat4 mProjectionMatrix;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCAMERA_H
