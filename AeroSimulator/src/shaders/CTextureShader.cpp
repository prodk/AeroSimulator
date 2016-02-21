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

   mElementsPerVertex = eElementsPerVertex::eTextureElements;
   mStride = eStride::eTextureStride;
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
   ///@todo: remove stride and elements per vertex from CGeometry from all the shaders!
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
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(float) * mStride,
      (const void*)(mElementsPerVertex * sizeof(float))); // Important!! Shift relative to the first array element

   glEnableVertexAttribArray(mTexCoordAttributeId);

   const glm::mat4 model = renderable.getMatrix4Param(eShaderMatrix4Params::MODEL_MATRIX);
   const glm::mat4 view = renderable.getMatrix4Param(eShaderMatrix4Params::VIEW_MATRIX);
   const glm::mat4 projection = renderable.getMatrix4Param(eShaderMatrix4Params::PROJECTION_MATRIX);
   const glm::mat4 MVP = projection * view * model;
   glUniformMatrix4fv(mMvpUniformId, 1, GL_FALSE, &MVP[0][0]);
}
