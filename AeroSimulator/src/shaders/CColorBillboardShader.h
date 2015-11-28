#ifndef AERO_SIMULATOR_CCOLOR_BILLBOARD_SHADER_H
#define AERO_SIMULATOR_CCOLOR_BILLBOARD_SHADER_H

#include "CShader.h"

namespace AeroSimulatorEngine
{
   class CColorBillboardShader : public CShader
   {
   public:
      CColorBillboardShader();
      virtual ~CColorBillboardShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   private:
      GLint mPositionAttribute;
      GLint mSquadAttribute;
      GLint mRightUniform;
      GLint mUpUniform;
      GLint mWidthUniform;
      GLint mHeightUniform;
      GLint mColorUniform;
      GLint mMvpUniform;
      GLint mShiftUniform;
   };
}
#endif // AERO_SIMULATOR_CCOLOR_BILLBOARD_SHADER_H
