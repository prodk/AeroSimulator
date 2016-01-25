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

   CLog::getInstance().log("* CColorShader created");
}

CColorShader::~CColorShader()
{
}

void CColorShader::link()
{
   if (!mIsLinked)
   {
      CShader::link();

      mPositionAttributeId = glGetAttribLocation(mProgramId, "aPosition");
      //CLog::getInstance().logGL("* CColorShader: glGetAttribLocation(mProgramId, aPosition): ");

      mColorUniformId = glGetUniformLocation(mProgramId, "uColor");
      //CLog::getInstance().logGL("* CColorShader: glGetAttribLocation(mProgramId, uColor): ");

      mMvpUniformId = glGetUniformLocation(mProgramId, "MVP");
      //CLog::getInstance().logGL("* CColorShader: glGetUniformLocation(mProgramId, MVP): ");

      mIsLinked = true;
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

      //const glm::vec4 color = renderable.getColor();
      //glUniform4fv(mColorUniformId, 1, &color[0]);

      //// Send the transformation to the currently bound shader in the "MVP" uniform
      //const glm::mat4 MVP = renderable.getMvpMatrix();
      //glUniformMatrix4fv(mMvpUniformId, 1, GL_FALSE, &MVP[0][0]);
   }
}
