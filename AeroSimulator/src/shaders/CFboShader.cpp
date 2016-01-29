#include "CFboShader.h"
#include "../CLog.h"
#include "../CRenderable.h"
#include "../CGeometry.h"
#include "../CTexture.h"

using namespace AeroSimulatorEngine;

CFboShader::CFboShader()
   : mPositionAttributeId(-1)
   , mTexCoordAttributeId(-1)
   , mSamplerUniformId(-1)
{
   mVertexShaderCode = readShader("../AeroSimulator/src/shaders/fbo.glslv");
   mFragmentShaderCode = readShader("../AeroSimulator/src/shaders/fbo.glslf");
}

CFboShader::~CFboShader()
{
}

void CFboShader::link()
{
   if (!mIsLinked)
   {
      CShader::link();

      mPositionAttributeId = glGetAttribLocation(mProgramId, "aPosition");
      CLog::getInstance().logGL("* CFboShader: glGetAttribLocation(mProgramId, aPosition): ");

      mTexCoordAttributeId = glGetAttribLocation(mProgramId, "aTexCoord");
      CLog::getInstance().logGL("* CFboShader: glGetAttribLocation(mProgramId, aTexCoord): ");

      mSamplerUniformId = glGetUniformLocation(mProgramId, "sTexture");
      CLog::getInstance().logGL("* CFboShader: glGetUniformLocation(mProgramId, sTexture): ");

      mIsLinked = true;
   }
}

void CFboShader::setup(CRenderable & renderable)
{
   const CGeometry* pGeometry = renderable.getGeometry();
   assert(pGeometry);

   CShader::setup(renderable);

   // Texture-specific part
   glActiveTexture((GLenum)renderable.get1DParam(TEXTURE_UNIT));
   CHECKGL("CFboShader: glActiveTexture() failed ");

   const GLint id = renderable.getTexture()->getId();
   glBindTexture(GL_TEXTURE_2D, id);
   glUniform1i(mSamplerUniformId, 0);

   glVertexAttribPointer(
      mPositionAttributeId,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(float) * pGeometry->getVertexStride(),
      0);

   glEnableVertexAttribArray(mPositionAttributeId);

   glVertexAttribPointer(
      mTexCoordAttributeId,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(float)*pGeometry->getVertexStride(),
      (const void*)(2 * sizeof(float))); // Important!! Shift relative to the first array element

   glEnableVertexAttribArray(mTexCoordAttributeId);
}
