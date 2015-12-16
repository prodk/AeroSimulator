// CShader.h - declaration of a base shader class

#ifndef AERO_SIMULATOR_CSHADER_H
#define AERO_SIMULATOR_CSHADER_H

#include "../include/glew.h"
#include "../include/wglew.h"
#include <gl/GL.h>
#include "../include/glext.h"

#include <string>

namespace AeroSimulatorEngine
{
   class CRenderable;

   class CShader
   {
   public:
      CShader();
      virtual ~CShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);
      bool isLinked() const { return mIsLinked; }

   protected:
      GLuint mVertexShaderId;
      GLuint mFragmentShaderId;
      GLint mProgramId;
      std::string mVertexShaderCode;
      std::string mFragmentShaderCode;
      bool mIsLinked;

      GLint loadShader(GLuint id, const std::string& shaderCode);
      std::string readShader(const char* filePath);
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CSHADER_H
