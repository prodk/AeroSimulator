#include "CColorLambertianShader.h"
#include "../CLog.h"
#include "../CRenderable.h"
#include "../CGeometry.h"

using namespace AeroSimulatorEngine;

CColorLambertianShader::CColorLambertianShader()
   : mNormalAttributeId(0)
   , mModelMatrixUniformId(0)
   , mAmbientUniformId(0)
   , mDiffuseUniformId(0)
   , mSunDirUniformId(0)
   , mEyePosUniformId(0)
{
   mVertexShaderCode =
      "attribute vec3 aPosition;\n"
      "attribute vec3 aNormal;\n"
      "uniform mat4 MVP;\n"
      "uniform mat4 uM;\n"
      "varying vec3 vEyeNormal;\n"
      "varying vec3 vPos;\n"
      "void main(){\n"
      "    vEyeNormal = (uM * vec4(aNormal, 0.0)).xyz;\n"
      //"    vEyeNormal = (gl_NormalMatrix * vec4(aNormal, 0.0)).xyz;\n"
      "    vPos = (uM * vec4(aPosition, 1.0)).xyz;\n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "}\n";

   mFragmentShaderCode =
      "uniform vec4 uColor;\n"
      "uniform vec3 uAmbient;\n"
      "uniform vec3 uDiffuse;\n"
      "uniform vec3 uSunDir;\n"         // Direction from the fragment to the sun
      "uniform vec3 uEyePos;\n"         // Camera position
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
      "    gl_FragColor = uColor * vec4(uAmbient + uDiffuse * cosD + uDiffuse * pow(cosS, 10), 1);\n"
      "}\n";

   CLog::getInstance().log("* CColorLambertianShader created");
}

CColorLambertianShader::~CColorLambertianShader()
{
}

void CColorLambertianShader::link()
{
   if (!mIsLinked)
   {
      CColorShader::link();

      mNormalAttributeId = glGetAttribLocation(mProgramId, "aNormal");
      CLog::getInstance().logGL("* CColorLambertianShader: glGetAttribLocation(mProgramId, aNormal): ");

      mModelMatrixUniformId = glGetUniformLocation(mProgramId, "uM");
      CLog::getInstance().logGL("* CColorLambertianShader: glGetUniformLocation(mProgramId, uM): ");

      mAmbientUniformId = glGetUniformLocation(mProgramId, "uAmbient");
      CLog::getInstance().logGL("* CColorLambertianShader: glGetUniformLocation(mProgramId, uAmbient): ");

      mDiffuseUniformId = glGetUniformLocation(mProgramId, "uDiffuse");
      CLog::getInstance().logGL("* CColorLambertianShader: glGetUniformLocation(mProgramId, uDiffuse): ");

      mSunDirUniformId = glGetUniformLocation(mProgramId, "uSunDir");
      CLog::getInstance().logGL("* CColorLambertianShader: glGetUniformLocation(mProgramId, uSunDir): ");

      mEyePosUniformId = glGetUniformLocation(mProgramId, "uEyePos");
      CLog::getInstance().logGL("* CColorLambertianShader: glGetUniformLocation(mProgramId, uEyePos): ");

      mIsLinked = true;
   }
}

void CColorLambertianShader::setup(CRenderable & renderable)
{
   CColorShader::setup(renderable);

   CGeometry* pGeometry = renderable.getGeometry();

   if (pGeometry)
   {
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

      const glm::vec3 ambientColor(0.2f, 0.1f, 0.0f);
      glUniform3fv(mAmbientUniformId, 1, &(ambientColor.r));

      const glm::vec3 diffuseColor(0.0f, 0.8f, 0.0f);
      glUniform3fv(mDiffuseUniformId, 1, &(diffuseColor.r));

      const glm::vec3 sunDirection(0.0f, 1.0f, 1.0f);
      glUniform3fv(mSunDirUniformId, 1, &(sunDirection.x));

      const glm::vec3 eyePos = renderable.getEyePos();
      glUniform3fv(mEyePosUniformId, 1, &eyePos.x);
   }
}
