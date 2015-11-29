#ifndef AERO_SIMULATOR_CCOLOR_BILLBOARD_SHADER_H
#define AERO_SIMULATOR_CCOLOR_BILLBOARD_SHADER_H

#include "CShader.h"

namespace AeroSimulatorEngine
{
   class CHealthbarShader : public CShader
   {
   public:
      CHealthbarShader();
      virtual ~CHealthbarShader();

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
      GLint mHealthUniform;
   };
}
#endif // AERO_SIMULATOR_CCOLOR_BILLBOARD_SHADER_H
