#include "CShader.h"
#include "../CRenderable.h"
#include "../CLog.h"
#include <iostream>
#include <fstream>
using namespace AeroSimulatorEngine;

CShader::CShader()
   : mVertexShaderId(0)
   , mFragmentShaderId(0)
   , mProgramId(0)
   , mVertexShaderCode()
   , mFragmentShaderCode()
   , mIsLinked(false)
{
   CLog::getInstance().log("CShader::CShader()");
}

CShader::~CShader()
{
}

void CShader::link()
{
   if (!mIsLinked)
   {
      mProgramId = glCreateProgram();

      // Vertex shader
      mVertexShaderId = glCreateShader(GL_VERTEX_SHADER);
      GLint status = loadShader(mVertexShaderId, mVertexShaderCode);
      CLog::getInstance().logGL("* Load vertex shader: ", status);

      // Fragment shader
      mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
      status = loadShader(mFragmentShaderId, mFragmentShaderCode);
      CLog::getInstance().logGL("* Load fragment shader: ", status);

      glLinkProgram(mProgramId);
      CLog::getInstance().logGL("* glLinkProgram(): ");

      mIsLinked = true;
   }
}

void CShader::setup(CRenderable & renderable)
{
   glUseProgram(mProgramId);
}

GLint CShader::loadShader(GLuint id, const std::string& shaderCode)
{
   const GLchar* pSourceCode = shaderCode.c_str();

   glShaderSource(id, 1, &pSourceCode, NULL);

   glCompileShader(id);
   CLog::getInstance().logGL("* glCompileShader(): ");

   glAttachShader(mProgramId, id);
   CLog::getInstance().logGL("* glAttachShader(): ");

   GLint status;
   glGetShaderiv(id, GL_COMPILE_STATUS, &status);

   return status;
}

std::string CShader::readShader(const char * filePath)
{
   std::string result;

   // Read and parse the file.
   std::ifstream fileIn(filePath);
   if (fileIn.good())
   {
      std::string line;
      while (std::getline(fileIn, line))
      {
         result += line;
      }

      fileIn.close();

      CLog::getInstance().log("CShader: successfully read the file: ", filePath);
   }
   else
   {
      CLog::getInstance().log("CShader: failed to read input file: ", filePath);
   }

   return result;
}
