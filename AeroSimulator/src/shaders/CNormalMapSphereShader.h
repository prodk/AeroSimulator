#ifndef AERO_SIMULATOR_CNORMAL_MAP_SPHERE_H
#define AERO_SIMULATOR_CNORMAL_MAP_SPHERE_H

#include "CColorShader.h"

#include <memory>

namespace AeroSimulatorEngine
{
   class CTexture;

   class CNormalMapSphereShader : public CColorShader
   {
   public:
      CNormalMapSphereShader();
      virtual ~CNormalMapSphereShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   private:
      GLint mSamplerUniformId;
   };
}

#endif // AERO_SIMULATOR_CNORMAL_MAP_SPHERE_H
