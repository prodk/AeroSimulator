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

   ///@todo: remove this
   //rotateCamera(renderable);
}

///@todo: remove this
//void CSimpleShader::rotateCameraGlm(CRenderable & renderable)
//{
//   // Init the View matrix
//   glm::mat4 View = glm::mat4(1.0f);
//   glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -9.0f);
//   View = glm::translate(View, cameraPos);
//
//   // Rotate the View matrix
//   static float angle = 0.f * DEG_TO_RAD;
//   static float angleX = 30.0f * DEG_TO_RAD;
//
//   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
//   //View = glm::rotate(View, angle, yAxis);
//   glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
//   View = glm::rotate(View, angleX, xAxis);
//
//   const float delta = 0.005f;
//   angle += delta;
//
//   ///@todo: move projection to construction
//   // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//   glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
//
//   ///@todo: take the model matrix from the CRenderable object.
//   ///@todo: move this to the CSimpleShader::setup()
//   // Model matrix : an identity matrix (model will be at the origin)
//   //glm::mat4 Model = glm::mat4(1.0f);
//   glm::mat4 Model = renderable.getModelMatrix();
//   glm::mat4 modelObject = glm::mat4(1.0f);
//   //modelObject = glm::rotate(modelObject, angle, yAxis);
//
//   glm::mat4 MVP = Projection * View * modelObject * Model;
//
//   // Send the transformation to the currently bound shader in the "MVP" uniform
//   glUniformMatrix4fv(mMvpAttributeId, 1, GL_FALSE, &MVP[0][0]);
//}
//
//void CSimpleShader::rotateCamera(CRenderable & renderable)
//{
//   rotateCameraGlm(renderable);
//}
