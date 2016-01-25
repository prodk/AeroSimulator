#ifndef AERO_SIMULATOR_C3DMODEL_H
#define AERO_SIMULATOR_C3DMODEL_H

#include <memory>
#include <vector>

#include "glm/vec3.hpp"

namespace AeroSimulatorEngine
{
   class CCompositeGameObject;
   class CParentGameObject;
   class CCube;
   class CGeometry;
   class CAxesFrame;
   class CShader;
   class CPropeller;
   class CBoundingBox;

   /// A bridge for the CGameObject Composite (tree structure)
   class C3DModel
   {
   public:
      C3DModel();
      ~C3DModel();

      bool buildModel(std::shared_ptr<CShader>& pShader);
      void getTree(std::vector<CCompositeGameObject*>& tree) const;

      /*CParentGameObject* getRoot() const { return mCabine.get(); }

      void setBillboardShader(std::shared_ptr<CShader>& pShader)
      {
         if (pShader) mBillboardShader = pShader;
      }*/

      /// Airplane's in the game
      void setPosition(const glm::vec3& pos) { mPosition = pos; }
      glm::vec3 getPosition() const { return mPosition; }

      void increasePropellerSpeed();
      void decreasePropellerSpeed();
      void setPropellerSpeed(const float speed);
      void resetPropellerSpeed();

      glm::vec3 getSpeedOfFlight() const { return mSpeedOfFlight; }
      void setSpeedOfFlight(const glm::vec3& speed) { mSpeedOfFlight = speed; }
      void resetSpeedOfFlight() { mSpeedOfFlight = mOriginalSpeedOfFlight; }

      glm::vec3 getDirectionOfFlight() const { return mXzDirection; }
      void setDirectionOfFlight(const glm::vec3& dir) { mXzDirection = dir; }
      void rotateFlightDirection(const float da, const float dt);

      void resetHealthBars(const float value = 0.0f);

      const CBoundingBox* getBoundingBox() const;

      float getHealth() const;

      float getFlightAngle() const { return mFlightAngle; }

      glm::vec3 getFlightDirection() const { return mXzDirection; }

      static const std::size_t numOfCubes;

   private:
      void setupCubeGeometry();
      void setupColorCubeGeometry();

   private:
      std::shared_ptr<CGeometry> mCubeGeometry;

      // Parts of the plane
      std::shared_ptr<CParentGameObject> mCabine;
      std::shared_ptr<CCompositeGameObject> mBody;
      std::shared_ptr<CCompositeGameObject> mLeftWing;
      std::shared_ptr<CCompositeGameObject> mRightWing;
      std::shared_ptr<CCompositeGameObject> mTail;
      std::shared_ptr<CPropeller> mPropeller;

      std::vector<CCube> mCubes;
      // Axis frames
      std::vector<CAxesFrame> mAxes;

      std::shared_ptr<CShader> mBillboardShader;

      // Position in the world space
      glm::vec3 mPosition;
      glm::vec3 mSpeedOfFlight;
      glm::vec3 mXzDirection; // Direction of flight in xz-plane

      std::shared_ptr<CBoundingBox> mBoundingBox;
      float mFlightAngle;
      glm::vec3 mOriginalSpeedOfFlight;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_C3DMODEL_H
