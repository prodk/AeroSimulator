#ifndef AERO_SIMULATOR_CTEXTURE_SHADER_H
#define AERO_SIMULATOR_CTEXTURE_SHADER_H

#include "CShader.h"

#include "../include/glew.h"
#include <gl/GL.h>

#include <memory>

namespace AeroSimulatorEngine
{
   class CTextureShader : public CShader
   {
   public:
      CTextureShader();
      virtual ~CTextureShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   protected:
      GLint mPositionAttributeId;
      GLint mTexCoordAttributeId;
      GLint mMvpUniformId;
      GLint mSamplerUniformId;
   };
} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CTEXTURE_SHADER_H
