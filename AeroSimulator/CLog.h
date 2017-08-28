// CLog.h - a logging class declaration

#ifndef AERO_SIMULATOR_CLOG_H
#define AERO_SIMULATOR_CLOG_H

#include <iostream>
#include "../AeroSimulator/include/glew.h"

#define LOG CLog::getInstance().log
#define LOGGL CLog::getInstance().logGL
#define CHECKGL CLog::getInstance().checkGL

namespace AeroSimulatorEngine
{
   class CLog
   {
   public:
      ~CLog();

      static CLog& getInstance()
      {
         static CLog instance;
         return instance;
      }

      ///@todo: add logging to some default log file
      void log(const char* msg, std::ostream& file = std::cout);
      void log(const char* msg, unsigned int value, std::ostream& file = std::cout);
      void log(const char* msg, int value0, float value1, std::ostream& file = std::cout);
      void log(const char* msg, const char* value, std::ostream& file = std::cout);
      void log(const char* msg1, const char* msg2, int value, std::ostream& file = std::cout);

      void logGL(const char* msg, std::ostream& file = std::cout);
      void logGL(const char* msg, GLint status, std::ostream& file = std::cout);
      void checkGL(const char* msg, std::ostream& file = std::cout);

      // Disable the copy constructor and copy assignment operator
      CLog(const CLog&) = delete;
      void operator=(const CLog&) = delete;

   private:
      CLog();
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLOG_H
