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
   mVertexShaderCode = readShader("../AeroSimulator/src/shaders/animationBillboard.glslv");

   // Fragment shader the same as for CTextureShader

   CLog::getInstance().log("* CBillboardShader created");
}

CAnimationBillboardShader::~CAnimationBillboardShader()
{
}

void CAnimationBillboardShader::link()
{
   if (!mIsLinked)
   {
      CBillboardShader::link();

      mCurrentFrameUniform = glGetUniformLocation(mProgramId, "uCurrentFrame");

      mFrameSizeUniform = glGetUniformLocation(mProgramId, "uFrameSize");

      mIsLinked = true;
   }
}

void CAnimationBillboardShader::setup(CRenderable & renderable)
{
   CBillboardShader::setup(renderable);

   // Frame number of the sprite
   //const glm::vec2 currentFrame = renderable.getCurrentFrame();
   //glUniform2fv(mCurrentFrameUniform, 1, &(currentFrame.x));

   //// Size of the sprite frame
   //const glm::vec2 frameSize = renderable.getFrameSize();
   //glUniform2fv(mFrameSizeUniform, 1, &(frameSize.x));
}
