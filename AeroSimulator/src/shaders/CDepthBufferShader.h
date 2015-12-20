#ifndef AERO_SIMULATOR_CDEPTH_BUFFER_SHADER_H
#define AERO_SIMULATOR_CDEPTH_BUFFER_SHADER_H

#include "CFboShader.h"

namespace AeroSimulatorEngine
{
   class CDepthBufferShader : public CFboShader
   {
   public:
      CDepthBufferShader();
      virtual ~CDepthBufferShader();
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CDEPTH_BUFFER_SHADER_H
