// CApp.h - the application class, it is a singleton

#ifndef AERO_SIMULATOR_CAPP_H
#define AERO_SIMULATOR_CAPP_H

#include "CTaskManager.h"

#include <iostream>
#include <memory>

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
   class CWin32Window;
   class CWin32Renderer;

   class CSimpleShader;
   class CGeometry;
   class CRenderable;

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
      // Disable the copy constructor and copy assignment operator
      CApp(const CApp&) = delete;
      void operator=(const CApp&) = delete;

      void setupRenderer();

   private:
      ///@todo: introduce a Bridge pattern and place Win32-specific code there
      CTaskManager mTaskManager;
      std::shared_ptr<CWin32Window> mAppWindowTask;
      std::shared_ptr<CWin32Renderer> mRendererTask;

      ///@todo: think about how to organize these members, probably the mGeometry and mShader are redundant.
      std::shared_ptr<CRenderable> mRenderable;
      std::shared_ptr<CGeometry> mGeometry;
      std::shared_ptr<CSimpleShader> mShader;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CAPP_H
