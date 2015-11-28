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
   , mSunPosUniformId(0)
   , mViewMatrixUniformId(0)
{
   mVertexShaderCode =
      "attribute vec3 aPosition;\n"
      "attribute vec3 aNormal;\n"
      "uniform mat4 MVP;\n"
      "uniform mat4 uM;\n"
      //"uniform mat4 uV;\n"         // View matrix
      "varying vec4 vEyeNormal;\n"
      "varying vec4 vPos;\n"
      "void main(){\n"
      "    vEyeNormal = uM * vec4(aNormal, 1.0);\n"
      "    vPos = uM * vec4(aPosition, 1.0);\n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "}\n";

   mFragmentShaderCode =
      "uniform vec4 uColor;\n"
      "uniform vec3 uAmbient;\n"
      "uniform vec3 uDiffuse;\n"
      "uniform vec3 uSunPos;\n"
      "varying vec4 vEyeNormal;\n"
      "varying vec4 vPos;\n"

      "const vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
      "const vec4 eyePos = vec4(0.0, 0.0, 13.0, 1.0);\n"
      "void main(){\n"
      "    vec4 N = normalize(vEyeNormal);\n"
      "    vec4 L = vec4(uSunPos, 1.0);\n"
      "    float cosD = clamp(dot(N, L), 0.0, 1.0);\n"
      "    vec4 cameraDir = normalize(eyePos - vPos);\n"

      //"    vec4 E = normalize(vEyeDirection);\n"
      "    vec4 R = reflect(-L, N);\n"
      "    float cosS = clamp(dot(cameraDir, R), 0.0, 1.0);\n"
      "    gl_FragColor = uColor * vec4(uAmbient + uDiffuse * cosD + pow(cosS, 20), 1);\n"
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

      mSunPosUniformId = glGetUniformLocation(mProgramId, "uSunPos");
      CLog::getInstance().logGL("* CColorLambertianShader: glGetUniformLocation(mProgramId, uSunPos): ");

      mViewMatrixUniformId = glGetUniformLocation(mProgramId, "uV");
      CLog::getInstance().logGL("* CColorLambertianShader: glGetUniformLocation(mProgramId, uV): ");

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

      const glm::vec3 diffuseColor(0.7f, 0.4f, 0.0f);
      glUniform3fv(mDiffuseUniformId, 1, &(diffuseColor.r));

      const glm::vec3 sunPosition(-1.0f, 0.0f, 0.0f);
      glUniform3fv(mSunPosUniformId, 1, &(sunPosition.x));

      ///@todo: remove
      const glm::mat4 uV = renderable.getViewMatrix();
      glUniformMatrix4fv(mViewMatrixUniformId, 1, GL_FALSE, &uV[0][0]);
   }
}
