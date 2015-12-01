#ifndef AERO_SIMULATOR_CANIMATION_BILLBOARD_SHADER_H
#define AERO_SIMULATOR_CANIMATION_BILLBOARD_SHADER_H

#include "CBillboardShader.h"

namespace AeroSimulatorEngine
{
   class CAnimationBillboardShader : public CBillboardShader
   {
   public:
      CAnimationBillboardShader();
      virtual ~CAnimationBillboardShader();

      virtual void link();
      virtual void setup(CRenderable & renderable);

   private:
      GLint mCurrentFrameUniform;
      GLint mFrameSizeUniform;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CANIMATION_BILLBOARD_SHADER_H
