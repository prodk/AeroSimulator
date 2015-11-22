#include "CBillboardShader.h"
#include "CLog.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CBillboardShader::CBillboardShader()
   : CTextureShader()
{
   mVertexShaderCode =
      "attribute vec3 aPosition;\n"
      "attribute vec2 aTexCoord;\n"
      "uniform mat4 MVP;\n"
      "varying vec2 vTexCoord;\n"
      "void main(){\n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "    vTexCoord = aTexCoord;\n"
      "}\n";

   mFragmentShaderCode =
      "precision highp float; \n"
      "varying vec2 vTexCoord;\n"
      "uniform sampler2D sTexture; \n"
      "void main(){\n"
      "    gl_FragColor = texture2D(sTexture, vTexCoord);\n"
      "}\n";

   assert(mTexture);

   CLog::getInstance().log("* CTextureShader created");
}

CBillboardShader::~CBillboardShader()
{
}

void CBillboardShader::link()
{
   CTextureShader::link();
   // Add uniform names here
}

void CBillboardShader::setup(CRenderable & renderable)
{
   CTextureShader::setup(renderable);
   // Set additional uniforms here
}
