// CLog.h - a logging class declaration

#ifndef AERO_SIMULATOR_CLOG_H
#define AERO_SIMULATOR_CLOG_H

#include <iostream>
#include "glew.h"

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
      void log(const char* msg, const char* value, std::ostream& file = std::cout);

      void logGL(const char* msg, std::ostream& file = std::cout);
      void logGL(const char* msg, GLint status, std::ostream& file = std::cout);

   private:
      CLog();
      // Disable the copy constructor and copy assignment operator
      CLog(const CLog&) = delete;
      void operator=(const CLog&) = delete;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLOG_H
