#include "CSimpleShader.h"
#include "CGeometry.h"
#include "CRenderable.h"
#include "CLog.h"

///@todo: probably remove some glm headers from here
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cassert>
#include <iostream>

using namespace AeroSimulatorEngine;

CSimpleShader::CSimpleShader()
   : mPositionAttributeId(0)
   , mColorAttributeId(0)
   , mMvpAttributeId(0)
{
   mVertexShaderCode =
      "attribute vec3 aPosition;\n"
      "attribute vec3 aColor;\n"
      "uniform mat4 MVP;\n"
      "varying vec4 vColor;\n"
      "void main(){\n"
      "    gl_Position = MVP * vec4(aPosition, 1.0);\n"
      "    vColor = vec4(aColor, 1.0f);\n"
      "}\n";

   mFragmentShaderCode =
      "varying vec4 vColor;\n"
      "void main(){\n"
      "    gl_FragColor = vColor;\n"
      "}\n";

   CLog::getInstance().log("* CSimpleShader::CSimpleShader(): created");
}

CSimpleShader::~CSimpleShader()
{
}

void CSimpleShader::link()
{
   CShader::link();

   mPositionAttributeId = glGetAttribLocation(mProgramId, "aPosition");
   CLog::getInstance().logGL("* glGetAttribLocation(mProgramId, aPosition): ");

   mColorAttributeId = glGetAttribLocation(mProgramId, "aColor");
   CLog::getInstance().logGL("* glGetAttribLocation(mProgramId, aColor): ");

   mMvpAttributeId = glGetUniformLocation(mProgramId, "MVP");
   CLog::getInstance().logGL("* glGetUniformLocation(mProgramId, MVP): ");
}

void CSimpleShader::setup(CRenderable & renderable)
{
   CShader::setup(renderable);

   CGeometry* pGeometry = renderable.getGeometry();
   assert(pGeometry);

   glVertexAttribPointer(
      mPositionAttributeId,
      pGeometry->getNumOfElementsPerVertex(),
      GL_FLOAT,
      GL_FALSE,
      sizeof(float) * pGeometry->getVertexStride(),
      0);

   glEnableVertexAttribArray(mPositionAttributeId);

   glVertexAttribPointer(
      mColorAttributeId,
      pGeometry->getNumOfElementsPerVertex(),
      GL_FLOAT,
      GL_FALSE,
      sizeof(float)*pGeometry->getVertexStride(),
      (const void*)(3*sizeof(float))); // Important!! Shift relative to the first array element

   glEnableVertexAttribArray(mColorAttributeId);

   // Send the transformation to the currently bound shader in the "MVP" uniform
   glm::mat4 MVP = renderable.getMvpMatrix();
   glUniformMatrix4fv(mMvpAttributeId, 1, GL_FALSE, &MVP[0][0]);
}

