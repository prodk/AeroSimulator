// CSimpleShader.h - a simple shader that just draws the scene with the basic transforms

#ifndef AERO_SIMULATOR_CSIMPLE_SHADER_H
#define AERO_SIMULATOR_CSIMPLE_SHADER_H

#include "CShader.h"

namespace AeroSimulatorEngine
{
#define M_PI           3.14159265358979323846f  /* pi */
#define DEG_TO_RAD M_PI / 180.f

   class CSimpleShader : public CShader
   {
   public:
      CSimpleShader();
      virtual ~CSimpleShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   private:
      void rotateCameraGlm(CRenderable & renderable);
      void rotateCamera(CRenderable & renderable);

   private:
      GLint mPositionAttributeId;
      GLint mColorAttributeId;
      GLint mMvpAttributeId;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CSIMPLE_SHADER_H

