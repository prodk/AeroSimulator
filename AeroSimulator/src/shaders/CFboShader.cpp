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

   mElementsPerVertex = eElementsPerVertex::eFboElements;
   mStride = eStride::eFboStride;
}

CFboShader::~CFboShader()
{
}

void CFboShader::link()
{
   if (!mIsLinked)
   {
      LOG("* CFboShader::link() start");
      CShader::link();

      mPositionAttributeId = glGetAttribLocation(mProgramId, "aPosition");
      CHECKGL("!* CFboShader: glGetAttribLocation(mProgramId, aPosition): failed: ");

      mTexCoordAttributeId = glGetAttribLocation(mProgramId, "aTexCoord");
      CHECKGL("!* CFboShader: glGetAttribLocation(mProgramId, aTexCoord): failed: ");

      mSamplerUniformId = glGetUniformLocation(mProgramId, "sTexture");
      CHECKGL("!* CFboShader: glGetUniformLocation(mProgramId, sTexture): failed: ");

      mIsLinked = true;
      LOG("* CFboShader::link() end");
      LOG("");
   }
}

void CFboShader::setup(CRenderable & renderable)
{
   CShader::setup(renderable);

   // Texture-specific part
   glActiveTexture((GLenum)renderable.get1DParam(TEXTURE_UNIT));
   CHECKGL("!* CFboShader: glActiveTexture() failed ");

   const GLint id = renderable.getTexture(MAIN_TEXTURE)->getId();
   glBindTexture(GL_TEXTURE_2D, id);
   glUniform1i(mSamplerUniformId, 0);

   glVertexAttribPointer(
      mPositionAttributeId,
      mElementsPerVertex,
      GL_FLOAT,
      GL_FALSE,
      sizeof(float) * mStride,
      0);

   glEnableVertexAttribArray(mPositionAttributeId);

   glVertexAttribPointer(
      mTexCoordAttributeId,
      mElementsPerVertex,
      GL_FLOAT,
      GL_FALSE,
      sizeof(float) * mStride,
      (const void*)(mElementsPerVertex * sizeof(float))); // Important!! Shift relative to the first array element

   glEnableVertexAttribArray(mTexCoordAttributeId);
}
