#ifndef AERO_SIMULATOR_C3DMODEL_H
#define AERO_SIMULATOR_C3DMODEL_H

#include <memory>
#include <vector>

namespace AeroSimulatorEngine
{
   class CCompositeGameObject;
   class CCube;
   class CGeometry;

   /// A bridge for the CGameObject Composite (tree structure)
   class C3DModel
   {
   public:
      C3DModel();
      ~C3DModel();

      bool buildModel();
      void getTree(std::vector<CCompositeGameObject*>& tree) const;

      CCompositeGameObject* getRoot() const { return mCabine.get(); }

      static const std::size_t numOfCubes;

   private:
      void setupCubeGeometry();

   private:
      std::shared_ptr<CGeometry> mCubeGeometry;

      // Parts of the plane
      std::shared_ptr<CCompositeGameObject> mCabine;
      std::shared_ptr<CCompositeGameObject> mBody;
      std::shared_ptr<CCompositeGameObject> mLeftWing;
      std::shared_ptr<CCompositeGameObject> mRightWing;
      std::shared_ptr<CCompositeGameObject> mTail;
      std::shared_ptr<CCompositeGameObject> mPropeller;

      std::vector<CCube> mCubes;
      // Axis frames
      std::shared_ptr<CCompositeGameObject> mAxes;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_C3DMODEL_H
