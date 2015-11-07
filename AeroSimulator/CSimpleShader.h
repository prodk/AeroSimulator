// CSimpleShader.h - a simple shader that just draws the scene with the basic transforms

#ifndef AERO_SIMULATOR_CSIMPLE_SHADER_H
#define AERO_SIMULATOR_CSIMPLE_SHADER_H

#include "CShader.h"

namespace AeroSimulatorEngine
{
   class CSimpleShader : public CShader
   {
   public:
      CSimpleShader();
      virtual ~CSimpleShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   private:
      void rotateCameraGlm();
      void rotateCamera();

   private:
      GLint mPositionAttributeId;
      GLint mColorAttributeId;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CSIMPLE_SHADER_H

