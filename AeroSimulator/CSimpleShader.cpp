#include "CSimpleShader.h"
#include "CGeometry.h"
#include "CRenderable.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cassert>

using namespace AeroSimulatorEngine;

CSimpleShader::CSimpleShader()
   : mPositionAttributeId(0)
{
   mVertexShaderCode =
      "attribute vec3 position;\n"
      "uniform mat4 MVP;\n"
      "varying vec4 color;\n"
      "void main(){\n"
      //"    color = vec4(position, 1.0);\n"
      //"    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
      "    gl_Position = MVP * vec4(position, 1.0);\n"
      //"    gl_Position = vec4(position, 1.0);\n"
      "    color = gl_Position;\n"
      "}\n";

   mFragmentShaderCode =
      "varying vec4 color;\n"
      "void main(){\n"
      "    gl_FragColor = color;\n"
      "}\n";
}

CSimpleShader::~CSimpleShader()
{
}

void CSimpleShader::link()
{
   CShader::link();

   mPositionAttributeId = glGetAttribLocation(mProgramId, "position");
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
      pGeometry->getVertexStride(),
      0
      /*pGeometry->getVertexBuffer()*/);

   glEnableVertexAttribArray(mPositionAttributeId);

   rotateCamera();
}

void CSimpleShader::rotateCamera()
{
   // Rotation stuff
   glm::mat4 View = glm::mat4(1.0f);
   glm::vec3 cameraPos = glm::vec3(0, 0, -3);
   View = glm::translate(View, cameraPos);

   const float delta = 0.01f;
   static float angle;
   glm::mat4 rotateCamera = glm::mat4(1.0f);
   glm::vec3 yAxis = glm::vec3(0, 1, 0);

   View = glm::rotate(View, angle, yAxis);

   angle += delta;

   // TODO: from tutorial
   // STEP 1: generate PVM matrices
   // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
   glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

   // Model matrix : an identity matrix (model will be at the origin)
   glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
                                       // Our ModelViewProjection : multiplication of our 3 matrices
   glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

                                              // STEP2: give the matrices to the shaders.
                                              // Get a handle for our "MVP" uniform.
                                              // Only at initialisation time.
   GLuint MatrixID = glGetUniformLocation(mProgramId, "MVP");

   // Send our transformation to the currently bound shader,
   // in the "MVP" uniform
   // For each model you render, since the MVP will be different (at least the M part)
   glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}
