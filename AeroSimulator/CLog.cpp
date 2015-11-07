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

void CLog::log(const char * msg, const char * value, std::ostream & file)
{
   file << msg << value << std::endl;
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
