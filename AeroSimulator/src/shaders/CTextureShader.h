#ifndef AERO_SIMULATOR_CTEXTURE_SHADER_H
#define AERO_SIMULATOR_CTEXTURE_SHADER_H

#include "CShader.h"

#include <memory>

namespace AeroSimulatorEngine
{
   class CTextureShader : public CShader
   {
   public:
      CTextureShader(const char * vertexPath = "../AeroSimulator/src/shaders/texture.glslv",
                     const char * fragPath = "../AeroSimulator/src/shaders/texture.glslf");
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
