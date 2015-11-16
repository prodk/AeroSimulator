#ifndef AERO_SIMULATOR_CTEXTURE_H
#define AERO_SIMULATOR_CTEXTURE_H

#include "../AeroSimulator/include/glew.h"
#include <gl/GL.h>

namespace AeroSimulatorEngine
{
   class CTexture
   {
   public:
      CTexture();
      ~CTexture();

      GLuint loadBmpTexture(const char * filePath);

   private:
      GLuint mId;
      GLuint mWidth;
      GLuint mHeight;
   };

} // namespace AeroSimulatorEngine
#endif AERO_SIMULATOR_CTEXTURE_H
