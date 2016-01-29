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
   CGeometry* pGeometry = renderable.getGeometry();

   if (pGeometry)
   {
      CShader::setup(renderable);

      glVertexAttribPointer(
         mPositionAttributeId,
         pGeometry->getNumOfElementsPerVertex(),
         GL_FLOAT,
         GL_FALSE,
         sizeof(float) * pGeometry->getVertexStride(),
         0);
      glEnableVertexAttribArray(mPositionAttributeId);

      ///@todo: get from renderable
      //const glm::vec4 color = renderable.getColor();
      const glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
      glUniform4fv(mColorUniformId, 1, &color[0]);

      //// Send the transformation to the currently bound shader in the "MVP" uniform
      //const glm::mat4 MVP = renderable.getMvpMatrix();
      const glm::mat4 MVP = renderable.getMatrix4Param(eShaderMatrix4Params::MVP_MATRIX);
      glUniformMatrix4fv(mMvpUniformId, 1, GL_FALSE, &MVP[0][0]);
   }
}
