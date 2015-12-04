#ifndef AERO_SIMULATOR_CCAMERA_H
#define AERO_SIMULATOR_CCAMERA_H

#include "CLeafCompositeGameObject.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"


namespace AeroSimulatorEngine
{
   class CCamera : public CLeafCompositeGameObject
   {
   public:
      CCamera();
      ~CCamera();

      void setProjectionMatrix(const glm::mat4& projection) { mProjectionMatrix = projection; }
      void setViewMatrix(const glm::mat4& view) { mViewMatrix = view; }
      void update();

      glm::mat4 getProjectionMatrix() const { return mProjectionMatrix; }
      glm::mat4 getViewMatrix() const { return mViewMatrix; }

      glm::vec3 getRightVector() const;
      glm::vec3 getUpVector() const;
      glm::vec3 getPositionWorldSpace() const;

      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);

   private:
      glm::mat4 mViewMatrix;
      glm::mat4 mProjectionMatrix;
      glm::mat4 mNonScaledViewMatrix;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCAMERA_H
