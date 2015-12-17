#include "CNormalMapSphereShader.h"
#include "../CLog.h"
#include "../CTexture.h"
#include "../CGeometry.h"
#include "../CRenderable.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CNormalMapSphereShader::CNormalMapSphereShader()
   : mNormalAttributeId(-1)
   , mTangentAttributeId(-1)
   , mModelMatrixUniformId(-1)
   , mSunDirUniformId(-1)
   , mEyePosUniformId(-1)
   , mSamplerUniformId(-1)
   , mNormalMapUniformId(-1)
   , mAnimationUniformId(-1)
   , mCurrentFrameUniform(-1)
   , mFrameSizeUniform(-1)
{
   mVertexShaderCode = readShader("../AeroSimulator/src/shaders/normalMapSphere.glslv");
   mFragmentShaderCode = readShader("../AeroSimulator/src/shaders/normalMapSphere.glslf");

   CLog::getInstance().log("* CNormalMapSphereShader created");
}

CNormalMapSphereShader::~CNormalMapSphereShader()
{
}

void CNormalMapSphereShader::link()
{
   if (!mIsLinked)
   {
      CColorShader::link();

      mNormalAttributeId = glGetAttribLocation(mProgramId, "aNormal");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetAttribLocation(mProgramId, aNormal): ");

      mTangentAttributeId = glGetAttribLocation(mProgramId, "aTangent");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetAttribLocation(mProgramId, aTangent): ");

      mModelMatrixUniformId = glGetUniformLocation(mProgramId, "uM");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetUniformLocation(mProgramId, uM): ");

      mSunDirUniformId = glGetUniformLocation(mProgramId, "uSunDir");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetUniformLocation(mProgramId, uSunDir): ");

      mEyePosUniformId = glGetUniformLocation(mProgramId, "uEyePos");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetUniformLocation(mProgramId, uEyePos): ");

      mSamplerUniformId = glGetUniformLocation(mProgramId, "sTexture");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetUniformLocation(mProgramId, sTexture): ");

      mNormalMapUniformId = glGetUniformLocation(mProgramId, "sNormalMap");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetUniformLocation(mProgramId, sNormalMap): ");

      mAnimationUniformId = glGetUniformLocation(mProgramId, "sAnimation");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetUniformLocation(mProgramId, sAnimation): ");

      mCurrentFrameUniform = glGetUniformLocation(mProgramId, "uCurrentFrame");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetUniformLocation(mProgramId, uCurrentFrame): ");

      mFrameSizeUniform = glGetUniformLocation(mProgramId, "uFrameSize");
      CLog::getInstance().logGL("* CNormalMapSphereShader: glGetUniformLocation(mProgramId, uFrameSize): ");

      mIsLinked = true;
   }
}

void CNormalMapSphereShader::setup(CRenderable & renderable)
{
   const CGeometry* pGeometry = renderable.getGeometry();
   if (pGeometry)
   {
      CColorShader::setup(renderable);

      /// Normals
      glVertexAttribPointer(
         mNormalAttributeId,
         3, // 3 coordinates per normal
         GL_FLOAT,
         GL_FALSE,
         sizeof(float)*pGeometry->getVertexStride(),
         (const void*)(3 * sizeof(float))); // Important!! Shift relative to the first array element
      glEnableVertexAttribArray(mNormalAttributeId);

      /// Tangents
      glVertexAttribPointer(
         mTangentAttributeId,
         3, // 3 coordinates per tangent
         GL_FLOAT,
         GL_FALSE,
         sizeof(float)*pGeometry->getVertexStride(),
         (const void*)(6 * sizeof(float))); // Important!! Shift relative to the first array element
      glEnableVertexAttribArray(mTangentAttributeId);

      const glm::mat4 uM = renderable.getModelMatrix();
      glUniformMatrix4fv(mModelMatrixUniformId, 1, GL_FALSE, &uM[0][0]);

      const glm::vec3 sunDirection(0.0f, 1.0f, 1.0f);
      glUniform3fv(mSunDirUniformId, 1, &(sunDirection.x));

      const glm::vec3 eyePos = renderable.getEyePos();
      glUniform3fv(mEyePosUniformId, 1, &eyePos.x);

      // Texture-specific part
      glActiveTexture(GL_TEXTURE0);
      const GLint id = renderable.getTexture()->getId();
      glBindTexture(GL_TEXTURE_2D, id);
      glUniform1i(mSamplerUniformId, 0);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      /// Normal map
      glActiveTexture(GL_TEXTURE1);
      const GLint mapId = renderable.getNormalMapTexture()->getId();
      glBindTexture(GL_TEXTURE_2D, mapId);
      glUniform1i(mNormalMapUniformId, 1);

      /// Animation texture
      glActiveTexture(GL_TEXTURE2);
      const GLint anId = renderable.getAnimationTexture()->getId();
      glBindTexture(GL_TEXTURE_2D, anId);
      glUniform1i(mAnimationUniformId, 2);

      // Frame number of the sprite
      const glm::vec2 currentFrame = renderable.getCurrentFrame();
      glUniform2fv(mCurrentFrameUniform, 1, &(currentFrame.x));

      // Size of the sprite frame
      const glm::vec2 frameSize = renderable.getFrameSize();
      glUniform2fv(mFrameSizeUniform, 1, &(frameSize.x));
   }
}
