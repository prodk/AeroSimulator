#include "CColorLambertianShader.h"
#include "../CLog.h"
#include "../CRenderable.h"
#include "../CGeometry.h"

using namespace AeroSimulatorEngine;

CColorLambertianShader::CColorLambertianShader()
{
   mVertexShaderCode =
      "attribute vec3 aPosition;\n"
      "attribute vec3 aNormal;\n"
      "uniform mat4 MVP;\n"
      "void main(){\n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "}\n";

   mFragmentShaderCode =
      "uniform vec4 uColor;\n"
      "void main(){\n"
      "    gl_FragColor = uColor;//vec4(1.0, 0.0, 0.0, 1.0); //uColor;\n"
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
   }
}
