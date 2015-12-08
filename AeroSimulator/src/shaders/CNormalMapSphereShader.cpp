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
   mVertexShaderCode =
      "float pi = 3.14159265f;\n"
      "float r = 1.0f;\n"
      "attribute vec3 aPosition;\n"
      "attribute vec3 aNormal;\n"
      "attribute vec3 aTangent;\n"
      "uniform mat4 MVP;\n"
      "uniform mat4 uM;\n"
      "uniform vec2 uCurrentFrame;  // Current frame in the sprite \n"
      "uniform vec2 uFrameSize;   // 1/numOfFrames in given direction\n"
      "varying vec2 vTexCoord;\n"
      "varying vec2 vTexCoordAnim;\n"
      "varying vec3 vEyeNormal;\n"  ///@todo: remove from varyings
      "varying vec3 vPos;\n"
      "varying mat3 mTBN;\n"
      "void main(){\n"
      "    vec3 Bworld = (uM * vec4(cross(aNormal, aTangent), 0.0)).xyz;\n"
      "    vec3 Tworld = (uM * vec4(aTangent, 0.0)).xyz;\n"
      "    vEyeNormal = (uM * vec4(aNormal, 0.0)).xyz;\n"
      "    mTBN = transpose(mat3(Tworld, Bworld, vEyeNormal));\n"
      "    vPos = (uM * vec4(aPosition, 1.0)).xyz;\n"
      "    vTexCoord.y = acos(aPosition.y/r)/(pi);\n"
      "    vTexCoord.x = (atan(aPosition.z, aPosition.x) + pi)/(2.0f*pi);\n"
      "    vTexCoordAnim = (vTexCoord + uCurrentFrame)*uFrameSize; \n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "}\n";

   mFragmentShaderCode =
      "vec3 uAmbient = vec3(0.3f, 0.3f, 0.3f);\n"
      "vec3 uDiffuse = vec3(0.8f, 0.8f, 0.8f);\n"
      "uniform vec3 uSunDir;\n"         // Direction from the fragment to the sun (world space)
      "uniform vec3 uEyePos;\n"         // Camera position world
      "uniform sampler2D sTexture; \n"
      "uniform sampler2D sNormalMap; \n"
      "uniform sampler2D sAnimation; \n"
      "uniform mat4 uM;\n"
      "varying vec2 vTexCoord;\n"
      "varying vec2 vTexCoordAnim;\n"
      "varying vec3 vEyeNormal;\n"      // Fragment normal in the world space
      "varying vec3 vPos;\n"
      "varying mat3 mTBN;\n"
      "void main(){\n"
      "    // Diffuse;\n"
      "    vec4 Nmodel = uM * vec4(normalize(texture2D( sNormalMap, vTexCoord ).rgb*2.0 - 1.0), 0.0);\n"
      "    vec3 N = Nmodel.xyz;\n"
      "    vec3 L = mTBN * normalize(uSunDir);\n"
      "    float cosD = clamp(dot(N, L), 0, 1);\n"
      "    // Specular;\n"
      "    float cosS = 0.0;\n"
      "    if (cosD > 0.0) {\n"
      "    vec3 cameraDir = mTBN * normalize(uEyePos - vPos);\n"
      "    vec3 R = reflect(-L, N);\n"
      "    cosS = clamp(dot(cameraDir, R), 0, 1);\n"
      "    }\n"
      "    gl_FragColor = (texture2D(sTexture, vTexCoord) + texture2D(sAnimation, vTexCoordAnim)) * vec4(uAmbient + uDiffuse * cosD + uDiffuse * pow(cosS, 10), 1);\n"
      //"    gl_FragColor = vec4(vTexCoord.x, vTexCoord.y, 0.0, 1.0);// * vec4(uAmbient + uDiffuse * cosD + uDiffuse * pow(cosS, 10), 1);\n"
      "}\n";

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
