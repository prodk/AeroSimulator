#include "CTextureShader.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

CTextureShader::CTextureShader()
{
   mVertexShaderCode =
      "attribute vec4 a_vPosition; \n"
      "attribute vec2 a_texCoord; \n"
      "varying vec2 v_texCoord; \n"
      "void main(){ \n"
      " gl_Position = a_vPosition; \n"
      " v_texCoord = a_texCoord; \n"
      "}\n";

   mFragmentShaderCode =
      "precision highp float; \n"
      "varying vec2 v_texCoord; \n"
      "uniform sampler2D s_texture; \n"
      "void main(){ \n"
      " gl_FragColor = texture2D(s_texture, v_texCoord); \n"
      "} \n";

   CLog::getInstance().log("* CTextureShader created");
}

CTextureShader::~CTextureShader()
{
}

void CTextureShader::link()
{
}

void CTextureShader::setup(CRenderable & renderable)
{
}
