#include "CShader.h"
#include "CRenderable.h"
#include <iostream>
using namespace AeroSimulatorEngine;

CShader::CShader()
   : mVertexShaderId(0)
   , mFragmentShaderId(0)
   , mProgramId(0)
   , mVertexShaderCode()
   , mFragmentShaderCode()
   , mIsLinked(false)
{
}

CShader::~CShader()
{
}

void CShader::link()
{
   mProgramId = glCreateProgram();

   mVertexShaderId = glCreateShader(GL_VERTEX_SHADER);

   ///@todo: make a uniform error logging method for OpenGL errors
   if (loadShader(mVertexShaderId, mVertexShaderCode))
   {
      std::cout << "* load vertex shader: success" << std::endl;
   }
   else
   {
      std::cout << "* load vertex shader: failed" << std::endl;
   }

   mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
   if(loadShader(mFragmentShaderId, mFragmentShaderCode))
   {
      std::cout << "* load fragment shader: success" << std::endl;
   }
   else
   {
      std::cout << "* load fragment shader: failed" << std::endl;
   }

   glLinkProgram(mProgramId);
   char str[256];
   GLenum err = glGetError();
   sprintf_s(str, "%s\n", glewGetErrorString(err));
   std::cout << "* glLinkProgram(): " << str << std::endl;

   ///@todo: log goes here

   mIsLinked = true;
}

void CShader::setup(CRenderable & renderable)
{
   glUseProgram(mProgramId);
}

bool CShader::loadShader(GLuint id, const std::string& shaderCode)
{
   const GLchar* pSourceCode = shaderCode.c_str();

   glShaderSource(id, 1, &pSourceCode, NULL);

   glCompileShader(id);

   glAttachShader(mProgramId, id);

   GLint status;
   glGetShaderiv(id, GL_COMPILE_STATUS, &status);

   return (GL_TRUE == status);
}
