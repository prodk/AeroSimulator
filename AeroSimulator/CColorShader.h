#ifndef AERO_SIMULATOR_CCOLOR_SHADER_H
#define AERO_SIMULATOR_CCOLOR_SHADER_H

#include "CShader.h"

namespace AeroSimulatorEngine
{
   class CColorShader : public CShader
   {
   public:
      CColorShader();
      virtual ~CColorShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   private:
      GLint mPositionAttributeId;
      GLint mColorUniformId;
      GLint mMvpUniformId;
   };
}
#endif // AERO_SIMULATOR_CCOLOR_SHADER_H
