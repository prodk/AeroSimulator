#include "CColorShader.h"
#include "../CLog.h"
#include "../CRenderable.h"
#include "../CGeometry.h"

using namespace AeroSimulatorEngine;

CColorShader::CColorShader()
   : mPositionAttributeId(0)
   , mMvpUniformId(0)
{
   mVertexShaderCode = readShader("../AeroSimulator/src/shaders/color.glslv");
   mFragmentShaderCode = readShader("../AeroSimulator/src/shaders/color.glslf");

   mElementsPerVertex = eElementsPerVertex::eTextureElements;
   mStride = eStride::eColorStride;
}

CColorShader::~CColorShader()
{
}

void CColorShader::link()
{
   if (!mIsLinked)
   {
      LOG("* CColorShader::link() start");
      CShader::link();

      mPositionAttributeId = glGetAttribLocation(mProgramId, "aPosition");
      CHECKGL("* CColorShader: glGetAttribLocation(mProgramId, aPosition) failed: ");

      mColorUniformId = glGetUniformLocation(mProgramId, "uColor");
      CHECKGL("* CColorShader: glGetUniformLocation(mProgramId, uColor) failed: ");

      mMvpUniformId = glGetUniformLocation(mProgramId, "MVP");
      CHECKGL("* CColorShader: glGetUniformLocation(mProgramId, MVP) failed: ");

      mIsLinked = true;
      LOG("* CColorShader::link() end");
      LOG("");
   }
}

void CColorShader::setup(CRenderable & renderable)
{
   CShader::setup(renderable);

   glVertexAttribPointer(
      mPositionAttributeId,
      mElementsPerVertex,
      GL_FLOAT,
      GL_FALSE,
      sizeof(float) * mStride,
      0);
   glEnableVertexAttribArray(mPositionAttributeId);

   const glm::vec4 color = renderable.getVector4Param(eShaderVector4Params::COLOR);
   glUniform4fv(mColorUniformId, 1, &color[0]);

   ///@todo: perform matrix multiplication on the GPU side
   const glm::mat4 model = renderable.getMatrix4Param(eShaderMatrix4Params::MODEL_MATRIX);
   const glm::mat4 view = renderable.getMatrix4Param(eShaderMatrix4Params::VIEW_MATRIX);
   const glm::mat4 projection = renderable.getMatrix4Param(eShaderMatrix4Params::PROJECTION_MATRIX);
   const glm::mat4 MVP = projection * view * model;
   glUniformMatrix4fv(mMvpUniformId, 1, GL_FALSE, &MVP[0][0]);
}
