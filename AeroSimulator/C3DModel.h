#ifndef AERO_SIMULATOR_C3DMODEL_H
#define AERO_SIMULATOR_C3DMODEL_H

#include <memory>
#include <vector>

#include "glm/vec3.hpp"

namespace AeroSimulatorEngine
{
   class CCompositeGameObject;
   class CCube;
   class CGeometry;
   class CAxesFrame;
   class CShader;
   class CPropeller;

   /// A bridge for the CGameObject Composite (tree structure)
   class C3DModel
   {
   public:
      C3DModel();
      ~C3DModel();

      bool buildModel(std::shared_ptr<CShader>& pShader);
      void getTree(std::vector<CCompositeGameObject*>& tree) const;

      CCompositeGameObject* getRoot() const { return mCabine.get(); }

      void setBillboardShader(std::shared_ptr<CShader>& pShader)
      {
         if (pShader) mBillboardShader = pShader;
      }

      void setPosition(const glm::vec3& pos) { mPosition = pos; }
      glm::vec3 getPosition() const { return mPosition; }

      void increasePropellerSpeed();
      void decreasePropellerSpeed();

      static const std::size_t numOfCubes;

   private:
      void setupCubeGeometry();
      void setupColorCubeGeometry();

   private:
      std::shared_ptr<CGeometry> mCubeGeometry;

      // Parts of the plane
      std::shared_ptr<CCompositeGameObject> mCabine;
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
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_C3DMODEL_H
