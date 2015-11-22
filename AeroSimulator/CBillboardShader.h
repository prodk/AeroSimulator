#ifndef AERO_SIMULATOR_CBILLBOARD_SHADER_H
#define AERO_SIMULATOR_CBILLBOARD_SHADER_H

#include "CTextureShader.h"

namespace AeroSimulatorEngine
{
   class CBillboardShader : public CTextureShader
   {
   public:
      CBillboardShader();
      virtual ~CBillboardShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   private:
      GLint mRightUniform;
      GLint mUpUniform;
      GLint mWidthUniform;
      GLint mHeightUniform;
   };
} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CBILLBOARD_SHADER_H
