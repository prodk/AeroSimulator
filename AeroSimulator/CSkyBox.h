#ifndef AERO_SIMULATOR_CSKYBOX_H
#define AERO_SIMULATOR_CSKYBOX_H

#include "CCube.h"

#include "../AeroSimulator/include/glew.h"
#include <gl/GL.h>

#include <memory>

namespace AeroSimulatorEngine
{
   class CGeometry;

   class CSkyBox : public CCube
   {
   public:
      CSkyBox();
      virtual ~CSkyBox();

      // Requires RC
      bool loadTexture(const char * fileName);
   };

} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CSKYBOX_H
