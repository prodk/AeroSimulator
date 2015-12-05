#include "CNormalMapSphereShader.h"
#include "../CLog.h"
#include "../CTexture.h"
#include "../CGeometry.h"
#include "../CRenderable.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CNormalMapSphereShader::CNormalMapSphereShader()
   : mSamplerUniformId(0)
{
   mVertexShaderCode =
      "attribute vec3 aPosition;\n"
      "uniform mat4 MVP;\n"
      "varying vec2 vTexCoord;\n"
      "float pi = 3.14159265f;\n"
      "float r = 1.0f;\n"
      "void main(){\n"
      "    vTexCoord.x = acos(aPosition.y/r)/(pi);\n"
      "    vTexCoord.y = (atan(aPosition.z, aPosition.x) + pi)/(2.0f*pi);\n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "}\n";

   mFragmentShaderCode =
      "precision highp float; \n"
      "varying vec2 vTexCoord;\n"
      "uniform sampler2D sTexture; \n"
      "void main(){\n"
      "    gl_FragColor = texture2D(sTexture, vTexCoord);\n"
      "}\n";

   CLog::getInstance().log("* CNormalMapSphereShader created");
}

CNormalMapSphereShader::~CNormalMapSphereShader()
{
}

void CNormalMapSphereShader::link()
{
   if (!mIsLinked)
   {
      CColorShader::link();

      mSamplerUniformId = glGetUniformLocation(mProgramId, "sTexture");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetUniformLocation(mProgramId, sTexture): ");
      mIsLinked = true;
   }
}

void CNormalMapSphereShader::setup(CRenderable & renderable)
{
   const CGeometry* pGeometry = renderable.getGeometry();
   if (pGeometry)
   {
      CColorShader::setup(renderable);

      // Texture-specific part
      glActiveTexture(GL_TEXTURE0);
      const GLint id = renderable.getTexture()->getId();
      glBindTexture(GL_TEXTURE_2D, id);
      glUniform1i(mSamplerUniformId, 0);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   }
}
