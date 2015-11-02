// CRenderable.h - renderable class declaration

#ifndef AERO_SIMULATOR_CRENDERABLE_H
#define AERO_SIMULATOR_CRENDERABLE_H

namespace AeroSimulatorEngine
{
   // Fwd declarations should be in the same namespace
   class CShader;
   class CGeometry;

   // CRenderable groups the geometry and its appearance in one entity
   class CRenderable
   {
   public:
      CRenderable();
      ~CRenderable();

      void setGeometry(CGeometry* pGeometry) { mGeometry = pGeometry; }
      CGeometry* getGeometry() const { return mGeometry; }

      void setShader(CShader* pShader) { mShader = pShader; }
      CShader* getShader() const { return mShader; }

   private:
      CGeometry* mGeometry;
      CShader* mShader;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERABLE_H
