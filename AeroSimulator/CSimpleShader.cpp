#include "CSimpleShader.h"
#include "CGeometry.h"
#include "CRenderable.h"
#include "CLog.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cassert>
#include <iostream>

using namespace AeroSimulatorEngine;

CSimpleShader::CSimpleShader()
   : mPositionAttributeId(0)
   , mColorAttributeId(0)
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
      0);

   glEnableVertexAttribArray(mColorAttributeId);

   rotateCamera();
}

void CSimpleShader::rotateCameraGlm()
{
   // Init the View matrix
   glm::mat4 View = glm::mat4(1.0f);
   glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -5.0f);
   View = glm::translate(View, cameraPos);

   // Rotate the View matrix
   static float angle;
   glm::mat4 rotateCamera = glm::mat4(1.0f);
   glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
   View = glm::rotate(View, angle, yAxis);
  /* glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
   View = glm::rotate(View, angle, xAxis);*/

   const float delta = 0.01f;
   angle += delta;

   // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
   glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

   // Model matrix : an identity matrix (model will be at the origin)
   glm::mat4 Model = glm::mat4(1.0f);
   glm::mat4 MVP = Projection * View * Model;

   // Give the matrix to the shaders.
   GLuint MatrixID = glGetUniformLocation(mProgramId, "MVP");

   // Send the transformation to the currently bound shader in the "MVP" uniform
   glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}

void AeroSimulatorEngine::CSimpleShader::rotateCamera()
{
   rotateCameraGlm();
}
