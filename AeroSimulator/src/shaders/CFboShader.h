#ifndef AERO_SIMULATOR_CFBO_SHADER_H
#define AERO_SIMULATOR_CFBO_SHADER_H

#include "CShader.h"

namespace AeroSimulatorEngine
{
   class CFboShader : public CShader
   {
   public:
      CFboShader();
      virtual ~CFboShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   protected:
      GLint mPositionAttributeId;
      GLint mTexCoordAttributeId;
      GLint mSamplerUniformId;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CFBO_SHADER_H
