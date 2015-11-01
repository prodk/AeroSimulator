// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CApp.h"
using namespace AeroSimulatorEngine;

// OpenGL extensions definitions
//PFNGLATTACHSHADERPROC glAttachShader;
//PFNGLBINDBUFFERPROC glBindBuffer;
//PFNGLBUFFERDATAPROC glBufferData;
//PFNGLCREATEPROGRAMPROC glCreateProgram;
//PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
//PFNGLGENBUFFERSPROC glGenBuffers;
//PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
//PFNGLLINKPROGRAMPROC glLinkProgram;
//PFNGLUSEPROGRAMPROC glUseProgram;
//PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
//
//PFNGLCOMPILESHADERPROC glCompileShader;
//PFNGLCREATESHADERPROC glCreateShader;
//PFNGLGETSHADERIVPROC glGetShaderiv;
//PFNGLSHADERSOURCEPROC glShaderSource;
//PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
//PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

int main()
{
   ///@todo: add non-default CApp constructor which initializes the app from some state.

   if (CApp::getInstance().init("AeroSimulator", 800, 600))
   {
      CApp::getInstance().run();
   }

   // TODO: debug
   /*std::cout << "* Video-system information:" << std::endl;
   std::cout << "  Videocard: " << (const char*)glGetString(GL_RENDERER) << std::endl;
   std::cout << "  Vendor: " << (const char*)glGetString(GL_VENDOR) << std::endl;
   std::cout << "  OpenGL Version: " << (const char*)glGetString(GL_VERSION) << std::endl << std::endl;*/
   // TODO: end

   return CApp::getInstance().exit();

   // TODO: fix the app cleanup when the Test.exe shuts down. Currently we get the error code 0xc000013a or smth like that. Should be 0.

   // Load shader manually.
   // TODO: check pointers for 0
   /*glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
   glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
   glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
   glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
   glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
   glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
   glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
   glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
   glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
   glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

   glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
   glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
   glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
   glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");

   glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
   glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");

   std::cout << "  OpenGL Extensions loaded manually. Stupid, but we have to do it this way.\n" << std::endl;*/
}

