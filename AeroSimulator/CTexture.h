#ifndef AERO_SIMULATOR_CTEXTURE_H
#define AERO_SIMULATOR_CTEXTURE_H

#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/wglew.h"
#include <gl/GL.h>
#include "../AeroSimulator/include/glext.h"

namespace AeroSimulatorEngine
{
   class CTexture
   {
   public:
      CTexture();
      ~CTexture();

      GLuint loadBmpTexture(const char * filePath);
      GLuint loadDDSTexture(const char * filePath);

      GLint getId() const { return mId; }

   private:
      GLuint mId;
      GLuint mWidth;
      GLuint mHeight;
   };

} // namespace AeroSimulatorEngine
#endif AERO_SIMULATOR_CTEXTURE_H
