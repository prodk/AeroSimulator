#include "CHealthbarShader.h"
#include "../CLog.h"
#include "../CRenderable.h"
#include "../CGeometry.h"

#include <cassert>

using namespace AeroSimulatorEngine;

CHealthbarShader::CHealthbarShader()
   : CShader()
   , mSquadAttribute(0)
   , mRightUniform(0)
   , mUpUniform(0)
   , mWidthUniform(0)
   , mHeightUniform(0)
   , mColorUniform(0)
   , mHealthUniform(0)
{
   mVertexShaderCode = readShader("../AeroSimulator/src/shaders/healthbar.glslv");
   mFragmentShaderCode = readShader("../AeroSimulator/src/shaders/healthbar.glslf");

   CLog::getInstance().log("* CColorBillboardShader created");
}

CHealthbarShader::~CHealthbarShader()
{
}

void CHealthbarShader::link()
{
   if (!mIsLinked)
   {
      CShader::link();

      mPositionAttribute = glGetAttribLocation(mProgramId, "aPosition");
      //CLog::getInstance().logGL("* CColorBillboardShader: glGetAttribLocation(mProgramId, aPosition): ");
      // Add uniform names here
      mSquadAttribute = glGetAttribLocation(mProgramId, "aSquad");
      //CLog::getInstance().logGL("* CColorBillboardShader: glGetAttribLocation(mProgramId, aSquad): ");

      mRightUniform = glGetUniformLocation(mProgramId, "uRight");
      //CLog::getInstance().logGL("* CColorBillboardShader: glGetUniformLocation(mProgramId, uRight): ");

      mUpUniform = glGetUniformLocation(mProgramId, "uUp");
      //CLog::getInstance().logGL("* CColorBillboardShader: glGetUniformLocation(mProgramId, uUp): ");

      mWidthUniform = glGetUniformLocation(mProgramId, "uWidth");
      //CLog::getInstance().logGL("* CColorBillboardShader: glGetUniformLocation(mProgramId, uWidth): ");

      mHeightUniform = glGetUniformLocation(mProgramId, "uHeight");
      //CLog::getInstance().logGL("* CColorBillboardShader: glGetUniformLocation(mProgramId, uHeight): ");

      mColorUniform = glGetUniformLocation(mProgramId, "uColor");
      //CLog::getInstance().logGL("* CColorBillboardShader: glGetUniformLocation(mProgramId, uColor): ");

      mMvpUniform = glGetUniformLocation(mProgramId, "MVP");
      //CLog::getInstance().logGL("* CColorShader: glGetUniformLocation(mProgramId, MVP): ");

      ///@todo: remove
      mHealthUniform = glGetUniformLocation(mProgramId, "uHealth");
      //CLog::getInstance().logGL("* CColorShader: glGetUniformLocation(mProgramId, uHealth): ");

      mIsLinked = true;
   }
}

void CHealthbarShader::setup(CRenderable & renderable)
{
   CShader::setup(renderable);

   const CGeometry* pGeometry = renderable.getGeometry();

   /// aSquad
   if (pGeometry)
   {
      glVertexAttribPointer(
         mPositionAttribute,
         pGeometry->getNumOfElementsPerVertex(),
         GL_FLOAT,
         GL_FALSE,
         sizeof(float) * pGeometry->getVertexStride(),
         0);
      glEnableVertexAttribArray(mPositionAttribute);

      // Set additional uniforms here
      glVertexAttribPointer(
         mSquadAttribute,
         2,
         GL_FLOAT,
         GL_FALSE,
         sizeof(float)*pGeometry->getVertexStride(),
         (const void*)(3 * sizeof(float))); // Shift relative to the first array element: 3 vertex 
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

   const glm::vec4 color = renderable.getColor();
   glUniform4fv(mColorUniform, 1, &color[0]);

   // Send the transformation to the currently bound shader in the "MVP" uniform
   const glm::mat4 MVP = renderable.getMvpMatrix();
   glUniformMatrix4fv(mMvpUniform, 1, GL_FALSE, &MVP[0][0]);

   ///@todo: remove
   const float health = renderable.getHealthValue();
   glUniform1f(mHealthUniform, health);
}
