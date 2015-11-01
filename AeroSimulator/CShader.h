// CShader.h - declaration of a base shader class

#ifndef AERO_SIMULATOR_CSHADER_H
#define AERO_SIMULATOR_CSHADER_H

#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/wglew.h"
#include <gl/GL.h>
#include "../AeroSimulator/include/glext.h"

#include <string>

//class CRenderable;

namespace AeroSimulatorEngine
{
   class CShader
   {
   public:
      CShader();
      virtual ~CShader();

      virtual void link();
      virtual void setup(/*CRenderable& renderable*/);
      bool isLinked() const { return mIsLinked; }

   protected:
      GLuint mVertexShaderId;
      GLuint mFragmentShaderId;
      GLint mProgramId;
      std::string mVertexShaderCode;
      std::string mFragmentShaderCode;
      bool mIsLinked;

      GLint mPositionAttributeId; // TODO: move to derived class

      bool loadShader(GLuint id, const std::string& shaderCode);
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CSHADER_H
