#include "CLog.h"

using namespace AeroSimulatorEngine;

void CLog::log(const char * msg, std::ostream & file)
{
   file << msg << std::endl;
}

void CLog::log(const char * msg, unsigned int value, std::ostream & file)
{
   file << msg << value << std::endl;
}

void CLog::log(const char * msg, int value0, float value1, std::ostream & file)
{
   file << msg << value0 << " " << value1 << std::endl;
}

void CLog::log(const char * msg, const char * value, std::ostream & file)
{
   file << msg << value << std::endl;
}

void CLog::log(const char* msg1, const char* msg2, int value, std::ostream& file)
{
   file << msg1 << msg2 << value << std::endl;
}

CLog::CLog()
{
}

CLog::~CLog()
{
}

void CLog::logGL(const char * msg, std::ostream & file)
{
   char str[256];
   const GLenum err = glGetError();
   sprintf_s(str, "%s", glewGetErrorString(err));
   file << msg << str << std::endl;
}

void CLog::logGL(const char * msg, GLint status, std::ostream & file)
{
   char str[256];
   sprintf_s(str, "%s", (GL_TRUE == status) ? "success" : "failed");
   file << msg << str << std::endl;
}

void CLog::checkGL(const char * msg, std::ostream & file)
{
   const GLenum err = glGetError();
   if (GL_NO_ERROR != err)
   {
      char str[256];
      sprintf_s(str, "%s", glewGetErrorString(err));
      file << msg << str << std::endl;
   }
}
