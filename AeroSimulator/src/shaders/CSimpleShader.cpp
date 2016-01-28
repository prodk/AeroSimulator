#include "CSimpleShader.h"
#include "../CGeometry.h"
#include "../CRenderable.h"
#include "../CLog.h"

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
   , mMvpUniformId(0)
{
   mVertexShaderCode = readShader("../AeroSimulator/src/shaders/simple.glslv");
   mFragmentShaderCode = readShader("../AeroSimulator/src/shaders/simple.glslf");

   CLog::getInstance().log("* CSimpleShader::CSimpleShader(): created");
}

CSimpleShader::~CSimpleShader()
{
}

void CSimpleShader::link()
{
   if (!mIsLinked)
   {
      CShader::link();

      mPositionAttributeId = glGetAttribLocation(mProgramId, "aPosition");
      CLog::getInstance().logGL("* CSimpleShader: glGetAttribLocation(mProgramId, aPosition): ");

      mColorAttributeId = glGetAttribLocation(mProgramId, "aColor");
      CLog::getInstance().logGL("* CSimpleShader: glGetAttribLocation(mProgramId, aColor): ");

      mMvpUniformId = glGetUniformLocation(mProgramId, "MVP");
      CLog::getInstance().logGL("* CSimpleShader: glGetUniformLocation(mProgramId, MVP): ");

      mIsLinked = true;
   }
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
   /*glm::mat4 MVP = renderable.getMvpMatrix();
   glUniformMatrix4fv(mMvpUniformId, 1, GL_FALSE, &MVP[0][0]);*/
}

