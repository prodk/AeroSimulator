#include "CColorShader.h"
#include "CLog.h"
#include "CRenderable.h"
#include "CGeometry.h"

using namespace AeroSimulatorEngine;

CColorShader::CColorShader()
   : mPositionAttributeId(0)
   , mMvpUniformId(0)
{
   mVertexShaderCode =
      "attribute vec3 aPosition;\n"
      "uniform mat4 MVP;\n"
      "void main(){\n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "}\n";

   mFragmentShaderCode =
      "uniform vec4 uColor;\n"
      "void main(){\n"
      "    gl_FragColor = uColor;//vec4(1.0, 0.0, 0.0, 1.0); //uColor;\n"
      "}\n";

   CLog::getInstance().log("* CSimpleShader::CSimpleShader(): created");
}

CColorShader::~CColorShader()
{
}

void CColorShader::link()
{
   CShader::link();

   mPositionAttributeId = glGetAttribLocation(mProgramId, "aPosition");
   CLog::getInstance().logGL("* CColorShader: glGetAttribLocation(mProgramId, aPosition): ");

   mColorUniformId = glGetUniformLocation(mProgramId, "uColor");
   CLog::getInstance().logGL("* CColorShader: glGetAttribLocation(mProgramId, uColor): ");

   mMvpUniformId = glGetUniformLocation(mProgramId, "MVP");
   CLog::getInstance().logGL("* CColorShader: glGetUniformLocation(mProgramId, MVP): ");
}

void CColorShader::setup(CRenderable & renderable)
{
   CShader::setup(renderable);

   CGeometry* pGeometry = renderable.getGeometry();

   if (pGeometry)
   {
      glVertexAttribPointer(
         mPositionAttributeId,
         pGeometry->getNumOfElementsPerVertex(),
         GL_FLOAT,
         GL_FALSE,
         sizeof(float) * pGeometry->getVertexStride(),
         0);
      glEnableVertexAttribArray(mPositionAttributeId);

      const glm::vec4 color = renderable.getColor();
      glUniform4fv(mColorUniformId, 1, &color[0]);

      // Send the transformation to the currently bound shader in the "MVP" uniform
      const glm::mat4 MVP = renderable.getMvpMatrix();
      glUniformMatrix4fv(mMvpUniformId, 1, GL_FALSE, &MVP[0][0]);
   }
}
