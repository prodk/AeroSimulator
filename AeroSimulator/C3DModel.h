#ifndef AERO_SIMULATOR_C3DMODEL_H
#define AERO_SIMULATOR_C3DMODEL_H

#include <memory>
#include <vector>

namespace AeroSimulatorEngine
{
   class CGameObject;
   class CCube;
   class CGeometry;

   ///This is a bridge for the CGameObject Composite (tree structure)
   class C3DModel
   {
   public:
      C3DModel();
      ~C3DModel();

      bool buildModel();
      void getTree(std::vector<CGameObject*>& tree) const;

      CGameObject* getRoot() const { return mCabine.get(); }

      static const std::size_t numOfCubes;

   private:
      void setupCubeGeometry();

   private:
      std::shared_ptr<CGeometry> mCubeGeometry;

      // Parts of the plane
      std::shared_ptr<CGameObject> mCabine;
      std::shared_ptr<CGameObject> mBody;
      std::shared_ptr<CGameObject> mLeftWing;
      std::shared_ptr<CGameObject> mRightWing;
      std::shared_ptr<CGameObject> mTail;
      std::shared_ptr<CGameObject> mPropeller;

      std::vector<CCube> mCubes;
   };
} // namespac AeroSimulatorEngine

#endif // AERO_SIMULATOR_C3DMODEL_H
