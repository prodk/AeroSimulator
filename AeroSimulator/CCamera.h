#ifndef AERO_SIMULATOR_CCAMERA_H
#define AERO_SIMULATOR_CCAMERA_H

#include "CGameObject.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

#include <vector>

namespace AeroSimulatorEngine
{
   class CTransform;

   struct SFrustum
   {
      SFrustum(const float fov, const float aspect, const float near, const float far);

      float mFov;
      float mAspect;
      float mNear;
      float mFar;
   };
   ///@note: camera transform is in the world space, not in the view (camera) space
   class CCamera : public CGameObject
   {
   public:
      CCamera(const int id, const int type, const CTransform& transform, const SFrustum& frustum);
      ~CCamera();

      void setId(const int id) { mId = id; }
      int getId() const { return mId; }

      ///@todo: move all the matrices to the Camera component
      glm::mat4 getViewMatrix(); ///@todo: probably make it const later
      glm::mat4 getProjectionMatrix();// const { return mProjectionMatrix; }

   private:
      CTransform& getTransform();
      void setEvents(std::vector<int>& events);

      //void registerEvents() const;
      //void attachEvents();

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
      //glm::mat4 mLookAtMatrix;  // Translates look at point relative to the parent
      int mId;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCAMERA_H
