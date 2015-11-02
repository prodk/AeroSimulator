// CRenderable.h - renderable class declaration

//#pragma once

#ifndef AERO_SIMULATOR_CRENDERABLE_H
#define AERO_SIMULATOR_CRENDERABLE_H

namespace AeroSimulatorEngine
{
   class CShader;
   class CGeometry;

   class CRenderable
   {
   public:
      CRenderable();
      ~CRenderable();

      void setGeometry(CGeometry* pGeometry) { mGeometry = pGeometry; }
      CGeometry* getGeometry() { return mGeometry; }

      void setShader(CShader* pShader) { mShader = pShader; }
      CShader* getShader() { return mShader; }

   private:
      CGeometry* mGeometry;
      CShader* mShader;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERABLE_H
