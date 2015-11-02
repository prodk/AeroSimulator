// CApp.h - the application class, it is a singleton

#ifndef AERO_SIMULATOR_CAPP_H
#define AERO_SIMULATOR_CAPP_H

#include "CAppWindow.h"
#include "CTaskManager.h"
#include "CWin32Renderer.h"

#include <iostream>
#include <memory>

//#include "glew.h"
//#include "wglew.h"
//#include <gl/GL.h>
//#include "../AeroSimulator/include/glext.h"

// OpenGL extensions to load manually
//extern PFNGLATTACHSHADERPROC glAttachShader;
//extern PFNGLBINDBUFFERPROC glBindBuffer;
//extern PFNGLBUFFERDATAPROC glBufferData;
//extern PFNGLCREATEPROGRAMPROC glCreateProgram;
//extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
//extern PFNGLGENBUFFERSPROC glGenBuffers;
//extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
//extern PFNGLLINKPROGRAMPROC glLinkProgram;
//extern PFNGLUSEPROGRAMPROC glUseProgram;
//extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
//
//extern PFNGLCOMPILESHADERPROC glCompileShader;
//extern PFNGLCREATESHADERPROC glCreateShader;
//extern PFNGLGETSHADERIVPROC glGetShaderiv;
//extern PFNGLSHADERSOURCEPROC glShaderSource;
//extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
//extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

namespace AeroSimulatorEngine
{
   class CApp
   {
   public:
      static CApp& getInstance()
      {
         static CApp instance;
         return instance;
      }

      ~CApp();

      ///@todo: later add passing app parameters read from a file here
      bool init(const char* name, unsigned int width, unsigned int height);
      void run();
      int exit();

   private:
      CApp();
      CApp(const CApp&) = delete;
      void operator=(const CApp&) = delete;

   private:
      CTaskManager mTaskManager;
      std::shared_ptr<CWin32Window> mAppWindowTask;
      std::shared_ptr<CWin32Renderer> mRendererTask;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPP_H
