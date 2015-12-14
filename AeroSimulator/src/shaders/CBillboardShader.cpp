#include "CBillboardShader.h"
#include "../CLog.h"
#include "../CRenderable.h"
#include "../CGeometry.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CBillboardShader::CBillboardShader()
   : CTextureShader()
   , mSquadAttribute(0)
   , mRightUniform(0)
   , mUpUniform(0)
   , mWidthUniform(0)
   , mHeightUniform(0)
{
   mVertexShaderCode =
      "attribute vec3 aPosition; // Center of the billboard\n"
      "attribute vec2 aTexCoord;\n"
      "attribute vec2 aSquad;\n"
      "uniform mat4 MVP;\n"
      "uniform vec3 uRight;    // Camera vector right\n"
      "uniform vec3 uUp;       // Camera vector up\n"
      "uniform float uWidth;   // Width of the billboard\n"
      "uniform float uHeight;  // Height of the billboard\n"
      "varying vec2 vTexCoord;\n"
      "void main(){\n"
      "    vec3 position = aPosition + uRight*aSquad.x * uWidth + uUp*aSquad.y*uHeight;\n" ///@todo: change to cross(up, right)
      "    gl_Position = MVP * vec4(position, 1.0);\n"
      "    vTexCoord = aTexCoord;\n"
      "}\n";

   // Fragment shader is the same as in CTextureShader

   CLog::getInstance().log("* CBillboardShader created");
}

CBillboardShader::~CBillboardShader()
{
}

void CBillboardShader::link()
{
   CTextureShader::link();
   // Add uniform names here
   mSquadAttribute = glGetAttribLocation(mProgramId, "aSquad");
   //CLog::getInstance().logGL("* CTextureShader: glGetAttribLocation(mProgramId, aSquad): ");

   mRightUniform = glGetUniformLocation(mProgramId, "uRight");
   //CLog::getInstance().logGL("* CTextureShader: glGetUniformLocation(mProgramId, uRight): ");

   mUpUniform = glGetUniformLocation(mProgramId, "uUp");
   //CLog::getInstance().logGL("* CTextureShader: glGetUniformLocation(mProgramId, uUp): ");

   mWidthUniform = glGetUniformLocation(mProgramId, "uWidth");
   //CLog::getInstance().logGL("* CTextureShader: glGetUniformLocation(mProgramId, uWidth): ");

   mHeightUniform = glGetUniformLocation(mProgramId, "uHeight");
   //CLog::getInstance().logGL("* CTextureShader: glGetUniformLocation(mProgramId, uHeight): ");
}

void CBillboardShader::setup(CRenderable & renderable)
{
   CTextureShader::setup(renderable);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

   const CGeometry* pGeometry = renderable.getGeometry();

   /// aSquad
   if (pGeometry)
   {
      // Set additional uniforms here
      glVertexAttribPointer(
         mSquadAttribute,
         2,
         GL_FLOAT,
         GL_FALSE,
         sizeof(float)*pGeometry->getVertexStride(),
         (const void*)(5 * sizeof(float))); // Shift relative to the first array element: 3 vertex + 2 tex coords
      glEnableVertexAttribArray(mSquadAttribute);
   }

   /// uRightUniform
   const glm::vec3 right = renderable.getRightVector();
   glUniform3fv(mRightUniform, 1, &(right.x));

   /// mUpUniform
   const glm::vec3 up = renderable.getUpVector();
   glUniform3fv(mUpUniform, 1, &(up.x));

   /// mWidthUniform
   const GLfloat width = renderable.getBillboardWidth();
   glUniform1f(mWidthUniform, width);

   /// mHeightUniform
   const GLfloat height = renderable.getBillboardHeight();
   glUniform1f(mHeightUniform, height);
}
