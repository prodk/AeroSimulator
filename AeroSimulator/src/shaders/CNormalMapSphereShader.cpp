#include "CNormalMapSphereShader.h"
#include "../CLog.h"
#include "../CTexture.h"
#include "../CGeometry.h"
#include "../CRenderable.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CNormalMapSphereShader::CNormalMapSphereShader()
   : mNormalAttributeId(-1)
   , mModelMatrixUniformId(-1)
   , mSunDirUniformId(-1)
   , mEyePosUniformId(-1)
   , mSamplerUniformId(-1)
   , mNormalMapUniformId(-1)
{
   //mVertexShaderCode =
   //   "attribute vec3 aPosition;\n"
   //   "uniform mat4 MVP;\n"
   //   "varying vec2 vTexCoord;\n"
   //   "float pi = 3.14159265f;\n"
   //   "float r = 1.0f;\n"
   //   "void main(){\n"
   //   "    vTexCoord.x = acos(aPosition.y/r)/(pi);\n"
   //   "    vTexCoord.y = (atan(aPosition.x, aPosition.z) + pi)/(2.0f*pi);\n"
   //   "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
   //   "}\n";

   //mFragmentShaderCode =
   //   "precision highp float; \n"
   //   "varying vec2 vTexCoord;\n"
   //   "uniform sampler2D sTexture; \n"
   //   "void main(){\n"
   //   "    gl_FragColor = texture2D(sTexture, vTexCoord);\n"
   //   //"    gl_FragColor = vec4(vTexCoord.x, vTexCoord.y, 0.0, 1.0);\n"
   //   "}\n";
   mVertexShaderCode =
      "float pi = 3.14159265f;\n"
      "float r = 1.0f;\n"
      "attribute vec3 aPosition;\n"
      "attribute vec3 aNormal;\n"
      "uniform mat4 MVP;\n"
      "uniform mat4 uM;\n"
      "varying vec2 vTexCoord;\n"
      "varying vec3 vEyeNormal;\n"
      "varying vec3 vPos;\n"
      "void main(){\n"
      "    vTexCoord.y = acos(aPosition.y/r)/(pi);\n"
      //"    if (aPosition.z == 0)\n"
      //"       vTexCoord.x = 0.75;\n"
      //"    else\n"
      "       vTexCoord.x = (atan(aPosition.z, aPosition.x) + pi)/(2.0f*pi);\n"
      "    vEyeNormal = (uM * vec4(aNormal, 0.0)).xyz;\n"
      "    vPos = (uM * vec4(aPosition, 1.0)).xyz;\n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "}\n";

   mFragmentShaderCode =
      "vec3 uAmbient = vec3(0.2f, 0.1f, 0.0f);\n"
      "vec3 uDiffuse = vec3(0.0f, 0.8f, 0.0f);\n"
      "uniform vec3 uSunDir;\n"         // Direction from the fragment to the sun
      "uniform vec3 uEyePos;\n"         // Camera position
      "uniform sampler2D sTexture; \n"
      "uniform sampler2D sNormalMap; \n"
      "varying vec2 vTexCoord;\n"
      "varying vec3 vEyeNormal;\n"      // Fragment normal in the world space
      "varying vec3 vPos;\n"
      "void main(){\n"
      "    // Diffuse;\n"
      "    vec3 N = normalize(vEyeNormal);\n"
      "    vec3 L = normalize(uSunDir);\n"
      "    float cosD = clamp(dot(N, L), 0, 1);\n"
      "    // Specular;\n"
      "    float cosS = 0.0;\n"
      "    vec3 cameraDir = normalize(uEyePos - vPos);\n"
      "    vec3 R = reflect(-L, N);\n"
      "    cosS = clamp(dot(cameraDir, R), 0, 1);\n"
      "    gl_FragColor = texture2D(sTexture, vTexCoord) * vec4(uAmbient + uDiffuse * cosD + uDiffuse * pow(cosS, 10), 1);\n"
      //"    gl_FragColor = texture2D(sNormalMap, vTexCoord) * vec4(uAmbient + uDiffuse * cosD + uDiffuse * pow(cosS, 10), 1);\n"
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

      mIsLinked = true;
   }
}

void CNormalMapSphereShader::setup(CRenderable & renderable)
{
   const CGeometry* pGeometry = renderable.getGeometry();
   if (pGeometry)
   {
      CColorShader::setup(renderable);

      glVertexAttribPointer(
         mNormalAttributeId,
         3, // 3 coordinates per normal
         GL_FLOAT,
         GL_FALSE,
         sizeof(float)*pGeometry->getVertexStride(),
         (const void*)(3 * sizeof(float))); // Important!! Shift relative to the first array element
      glEnableVertexAttribArray(mNormalAttributeId);

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
   }
}
