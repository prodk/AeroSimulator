#include "CAnimationBillboardShader.h"
#include "../CLog.h"
#include "../CRenderable.h"

#include <cassert>

using namespace AeroSimulatorEngine;

///@todo: probably make CBillboardShader an animation shader
///@todo: simply use uFrameShift == 0 by default and do not change it
CAnimationBillboardShader::CAnimationBillboardShader()
   : CBillboardShader()
   , mCurrentFrameUniform(0)
   , mFrameSizeUniform(0)
{
   mVertexShaderCode =
      "attribute vec3 aPosition; // Center of the billboard\n"
      "attribute vec2 aTexCoord;\n"
      "attribute vec2 aSquad;\n"
      "uniform mat4 MVP;\n"
      "uniform vec3 uRight;    // Camera vector right\n"
      "uniform vec3 uUp;       // Camera vector up\n"
      "uniform float uWidth;   // Width of the billboard\n"
      "uniform float uHeight;  // Height of the billboard\n"
      "uniform vec2 uCurrentFrame;  // Current frame in the sprite \n"
      "uniform vec2 uFrameSize;   // 1/numOfFrames in given direction\n"
      "varying vec2 vTexCoord;\n"
      "void main(){\n"
      "    vec3 position = aPosition + uRight*aSquad.x * uWidth + vec3(0.0, 1.0, 0.0)*aSquad.y*uHeight;\n"
      "    gl_Position = MVP * vec4(position, 1.0);\n"
      "    vTexCoord = (aTexCoord + uCurrentFrame)*uFrameSize;\n"
      "}\n";

   // Fragment shader the same as for CTextureShader

   CLog::getInstance().log("* CBillboardShader created");
}

CAnimationBillboardShader::~CAnimationBillboardShader()
{
}

void CAnimationBillboardShader::link()
{
   CBillboardShader::link();

   mCurrentFrameUniform = glGetUniformLocation(mProgramId, "uCurrentFrame");

   mFrameSizeUniform = glGetUniformLocation(mProgramId, "uFrameSize");
}

void CAnimationBillboardShader::setup(CRenderable & renderable)
{
   CBillboardShader::setup(renderable);

   // Frame number of the sprite
   const glm::vec2 currentFrame = renderable.getCurrentFrame();
   glUniform2fv(mCurrentFrameUniform, 1, &(currentFrame.x));

   // Size of the sprite frame
   const glm::vec2 frameSize = renderable.getFrameSize();
   glUniform2fv(mFrameSizeUniform, 1, &(frameSize.x));
}
