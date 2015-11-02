#include "CSimpleShader.h"
#include "CGeometry.h"
#include "CRenderable.h"
#include <cassert>

using namespace AeroSimulatorEngine;

CSimpleShader::CSimpleShader()
   : mPositionAttributeId(0)
{
   mVertexShaderCode =
      "#version 150\n"
      "attribute vec3 position;\n"
      "uniform mat4 MVP;\n"
      "varying vec4 color;\n"
      "void main(){\n"
      //"    color = vec4(position, 1.0);\n"
      //"    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
      "    gl_Position = MVP * vec4(position, 1.0);\n"
      "    color = gl_Position;\n"
      "}\n";

   mFragmentShaderCode =
      "#version 150\n"
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
      pGeometry->getNumOfVertexPositions(),
      GL_FLOAT,
      GL_FALSE,
      pGeometry->getVertexStride(),
      pGeometry->getVertexBuffer());

   glEnableVertexAttribArray(mPositionAttributeId);
}
