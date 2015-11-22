#include "CTextureShader.h"
#include "CRenderable.h"
#include "CGeometry.h"
#include "CTexture.h"
#include "CLog.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CTextureShader::CTextureShader()
   : mTexture(new CTexture())
   , mPositionAttributeId(0)
   , mTexCoordAttributeId(0)
   , mMvpAttributeId(0)
   , mSamplerUniformId(0)
{
   mVertexShaderCode =
      "attribute vec3 aPosition;\n"
      "attribute vec2 aTexCoord;\n"
      "uniform mat4 MVP;\n"
      "varying vec2 vTexCoord;\n"
      "void main(){\n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "    vTexCoord = aTexCoord;\n"
      "}\n";

   mFragmentShaderCode =
      "precision highp float; \n"
      "varying vec2 vTexCoord;\n"
      "uniform sampler2D sTexture; \n"
      "void main(){\n"
      "    gl_FragColor = texture2D(sTexture, vTexCoord);\n"
      "}\n";

   assert(mTexture);

   CLog::getInstance().log("* CTextureShader created");
}

CTextureShader::~CTextureShader()
{
}

void CTextureShader::link()
{
   CShader::link();

   mPositionAttributeId = glGetAttribLocation(mProgramId, "aPosition");
   CLog::getInstance().logGL("* CTextureShader: glGetAttribLocation(mProgramId, aPosition): ");

   mTexCoordAttributeId = glGetAttribLocation(mProgramId, "aTexCoord");
   CLog::getInstance().logGL("* CTextureShader: glGetAttribLocation(mProgramId, aTexCoord): ");

   mMvpAttributeId = glGetUniformLocation(mProgramId, "MVP");
   CLog::getInstance().logGL("* CTextureShader: glGetUniformLocation(mProgramId, MVP): ");

   mSamplerUniformId = glGetUniformLocation(mProgramId, "sTexture");
   CLog::getInstance().logGL("* CTextureShader: glGetUniformLocation(mProgramId, sTexture): ");
}

void CTextureShader::setup(CRenderable & renderable)
{
   CGeometry* pGeometry = renderable.getGeometry();
   assert(pGeometry);
   assert(mTexture);

   CShader::setup(renderable);

   // Texture-specific part
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, renderable.getTexture()->getId());
   glUniform1i(mSamplerUniformId, 0);

   ///@todo: use mip maps later
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glVertexAttribPointer(
      mPositionAttributeId,
      3,
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
      (const void*)(3 * sizeof(float))); // Important!! Shift relative to the first array element

   glEnableVertexAttribArray(mTexCoordAttributeId);

   // Send the transformation to the currently bound shader in the "MVP" uniform
   glm::mat4 MVP = renderable.getMvpMatrix();
   glUniformMatrix4fv(mMvpAttributeId, 1, GL_FALSE, &MVP[0][0]);
}
