#ifndef AERO_SIMULATOR_CCOLOR_LAMBERTIAN_SHADER_H
#define AERO_SIMULATOR_CCOLOR_LAMBERTIAN_SHADER_H

#include "CColorShader.h"

namespace AeroSimulatorEngine
{
   class CColorLambertianShader : public CColorShader
   {
   public:
      CColorLambertianShader();
      virtual ~CColorLambertianShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   protected:
      GLint mNormalAttributeId;
      GLint mModelMatrixUniformId;
      GLint mAmbientUniformId;
      GLint mDiffuseUniformId;
      GLint mSunDirUniformId;
      GLint mEyePosUniformId;
   };
} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CCOLOR_LAMBERTIAN_SHADER_H
