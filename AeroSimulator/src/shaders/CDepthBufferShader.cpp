#include "CDepthBufferShader.h"

using namespace AeroSimulatorEngine;

CDepthBufferShader::CDepthBufferShader()
{
   // Vertex shader is the same as for CFboShader
   mFragmentShaderCode = readShader("../AeroSimulator/src/shaders/depthBuffer.glslf");
}


CDepthBufferShader::~CDepthBufferShader()
{
}
