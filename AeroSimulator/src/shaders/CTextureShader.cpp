#include "CTextureShader.h"
#include "../CRenderable.h"
#include "../CGeometry.h"
#include "../CTexture.h"
#include "../CLog.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CTextureShader::CTextureShader()
   : mPositionAttributeId(0)
   , mTexCoordAttributeId(0)
   , mMvpUniformId(0)
   , mSamplerUniformId(0)
{
   mVertexShaderCode = readShader("../AeroSimulator/src/shaders/texture.glslv");
   mFragmentShaderCode = readShader("../AeroSimulator/src/shaders/texture.glslf");
}

CTextureShader::~CTextureShader()
{
}

void CTextureShader::link()
{
   if (!mIsLinked)
   {
      LOG("* CTextureShader::link() start");
      CShader::link();

      mPositionAttributeId = glGetAttribLocation(mProgramId, "aPosition");
      CHECKGL("* CTextureShader: glGetAttribLocation(mProgramId, aPosition) failed: ");

      mTexCoordAttributeId = glGetAttribLocation(mProgramId, "aTexCoord");
      CHECKGL("* CTextureShader: glGetAttribLocation(mProgramId, aTexCoord) failed: ");

      mMvpUniformId = glGetUniformLocation(mProgramId, "MVP");
      CHECKGL("* CTextureShader: glGetUniformLocation(mProgramId, MVP) failed: ");

      mSamplerUniformId = glGetUniformLocation(mProgramId, "sTexture");
      CHECKGL("* CTextureShader: glGetUniformLocation(mProgramId, sTexture) failed: ");

      mIsLinked = true;
      LOG("* CTextureShader::link() end");
      LOG("");
   }
}

void CTextureShader::setup(CRenderable & renderable)
{
   const CGeometry* pGeometry = renderable.getGeometry();
   assert(pGeometry);

   CShader::setup(renderable);

   // Texture-specific part
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, renderable.getTexture(MAIN_TEXTURE)->getId());
   glUniform1i(mSamplerUniformId, 0);

   if (renderable.getFlag(eShaderFlags::REPEAT_TEXTURE))
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   }
   else
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   }
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   const int stride = pGeometry->getVertexStride();
   glVertexAttribPointer(
      mPositionAttributeId,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(float) * stride,
      0);

   glEnableVertexAttribArray(mPositionAttributeId);

   glVertexAttribPointer(
      mTexCoordAttributeId,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(float) * stride,
      (const void*)(3 * sizeof(float))); // Important!! Shift relative to the first array element

   glEnableVertexAttribArray(mTexCoordAttributeId);

   // Send the transformation to the currently bound shader in the "MVP" uniform
   const glm::mat4 MVP = renderable.getMatrix4Param(eShaderMatrix4Params::MVP_MATRIX);
   glUniformMatrix4fv(mMvpUniformId, 1, GL_FALSE, &MVP[0][0]);
}
