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

   //GLint mPositionAttributeId = glGetAttribLocation(mProgramId, "position"); // TODO: move to derived

   mIsLinked = true;
}

void CShader::setup(CRenderable & renderable)
{
   glUseProgram(mProgramId);

   // TODO: For testing:
   // Cube
   //float cubeData[] = {
   //   // Bottom plane
   //   -0.5f, -0.5f, 0.5f,  //0
   //   -0.5f, -0.5f, -0.5f, //1
   //   0.5f, -0.5f, -0.5f, //2
   //   0.5f, -0.5f, 0.5f, //3

   //                      // Top plane
   //   -0.5f, 0.5f, -0.5f, //4
   //   0.5f, 0.5f, -0.5f, //5
   //   0.5f, 0.5f, 0.5f, //6
   //   -0.5f, 0.5f, 0.5f, //7
   //};

   //// Generate a VBO
   //GLuint mVboId = 0;
   //glGenBuffers(1, &mVboId);
   //glBindBuffer(GL_ARRAY_BUFFER, mVboId);
   ////Log::instance().logStatus("* glBindBuffer() VBO: ");

   //glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);

   //   glVertexAttribPointer(mPositionAttributeId, 3, GL_FLOAT, GL_FALSE, 0, 0);
   //glEnableVertexAttribArray(mPositionAttributeId);

   //GLuint elements[] = {
   //   6, 7, 3, 0, 2, 1, 5, 4, 6, 7
   //};

   //GLuint ebo;
   //glGenBuffers(1, &ebo);
   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   ////Log::instance().logStatus("* glBindBuffer() Index buffer: ");

   //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
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
