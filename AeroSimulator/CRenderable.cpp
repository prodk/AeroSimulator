#include "CRenderable.h"
#include "CGeometry.h"
#include "../src/shaders/CShader.h"
#include "CLog.h"
//#include "CCommonMath.h"

//#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

CRenderable::CRenderable()
   : mGeometry()
   , mShader()
   /*, mModelMatrix()
   , mMvpMatrix()*/
   , mTexture()
   //, mVboId(0)
   //, mIboId(0)
   //, mRightVector()
   //, mUpVector()
   //, mBillboardWidth(1.0f)
   //, mBillboardHeight(1.0f)
   //, mDrawWithLines(false)
   //, mColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
   //, mLineWidth(2.0f)
   //, mHealthValue(0.0f)
   //, mViewMatrix()
   //, mEyePos()
   //, mCurrentFrame()
   //, mFrameSize()
   //, mNumOfFrames()
   //, mIsVisible(true)
   , mNormalMapTexture()
   , mAnimationTexture()
   //, mRepeatTexture(false)
   //, mIsTransparent(false)
   //, mTextureUnit(GL_TEXTURE0)
{
}

CRenderable::~CRenderable()
{
   mGeometry.reset();
   mShader.reset();
   mTexture.reset();
}

void CRenderable::resetEnvironment()
{
}

void CRenderable::setEnvironment()
{
}

void CRenderable::setFlag(const int id, const bool value)
{
   const std::pair<int, bool> element(id, value);
   mFlags.insert(element);
}

void CRenderable::set1DParam(const int id, const float value)
{
   const std::pair<int, float> element(id, value);
   m1DParams.insert(element);
}

void CRenderable::setVector2Param(const int id, const glm::vec2& value)
{
   const std::pair<int, glm::vec2> element(id, value);
   mVector2Params.insert(element);
}

void CRenderable::setVector3Param(const int id, const glm::vec3& value)
{
   const std::pair<int, glm::vec3> element(id, value);
   mVector3Params.insert(element);
}

void CRenderable::setVector4Param(const int id, const glm::vec4 & value)
{
   const std::pair<int, glm::vec4> element(id, value);
   mVector4Params.insert(element);
}

void CRenderable::setMatrix3Param(const int id, const glm::mat3 & value)
{
   const std::pair<int, glm::mat3> element(id, value);
   mMatrix3Params.insert(element);
}

void CRenderable::setMatrix4Param(const int id, const glm::mat4& value)
{
   const std::pair<int, glm::mat4> element(id, value);
   mMatrix4Params.insert(element);
}

bool CRenderable::getFlag(const int id) const
{
   ///@todo: try when compiles findValueInMap<bool>(mFlags, id)
   auto result = false;

   auto iter = mFlags.find(id);
   if (mFlags.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      CLog::getInstance().log("Failed to find a flag: ", id);
   }

   return result;
}

float CRenderable::get1DParam(const int id) const
{
   ///@todo: try when compiles return findValueInMap<float>(m1DParams, id);
   auto result = 0.0f;

   auto iter = m1DParams.find(id);
   if (m1DParams.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      CLog::getInstance().log("Failed to find 1D param: ", id);
   }

   return result;
}

glm::vec2 CRenderable::getVector2Param(const int id) const
{
   ///@todo: when compiles try:
   //return findValueInMap<glm::vec2>(m2DParams, id);
   auto result = glm::vec2();

   auto iter = mVector2Params.find(id);
   if (mVector2Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      CLog::getInstance().log("Failed to find vector 2D: ", id);
   }

   return result;
}

glm::vec3 CRenderable::getVector3Param(const int id) const
{
   ///@todo: when compiles try:
   //return findValueInMap<glm::vec3>(m3DParams, id);
   auto result = glm::vec3();

   auto iter = mVector3Params.find(id);
   if (mVector3Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      CLog::getInstance().log("Failed to find vector 3D: ", id);
   }

   return result;
}

glm::vec4 CRenderable::getVector4Param(const int id) const
{
   ///@todo: when compiles try:
   //return findValueInMap<glm::vec4>(mVector4Params, id);
   auto result = glm::vec4();

   auto iter = mVector4Params.find(id);
   if (mVector4Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      CLog::getInstance().log("Failed to find vector 4D: ", id);
   }

   return result;
}

glm::mat3 CRenderable::getMatrix3Param(const int id) const
{
   ///@todo: when compiles try:
   //return findValueInMap<glm::mat4>(m4DParams, id);
   auto result = glm::mat3();

   auto iter = mMatrix3Params.find(id);
   if (mMatrix3Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      CLog::getInstance().log("Failed to find matrix 3D: ", id);
   }

   return result;
}

glm::mat4 CRenderable::getMatrix4Param(const int id) const
{
   ///@todo: when compiles try:
   //return findValueInMap<glm::mat4>(m4DParams, id);
   auto result = glm::mat4();

   auto iter = mMatrix4Params.find(id);
   if (mMatrix4Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      CLog::getInstance().log("Failed to find matrix 4D: ", id);
   }

   return result;
}

bool CRenderable::loadTexture(const char * filePath)
{
   return false;
}

bool CRenderable::loadNormalMapTexture(const char * filePath)
{
   return false;
}

bool CRenderable::loadAnimationTexture(const char * filePath)
{
   return false;
}

//glm::mat3 CRenderable::getNormalMatrix() const
//{
//   glm::mat3x3 result;
//   CCommonMath::copyColumn(0, result, mModelMatrix);
//   CCommonMath::copyColumn(1, result, mModelMatrix);
//   CCommonMath::copyColumn(2, result, mModelMatrix);
//
//   result = glm::inverse(result);
//   result = glm::transpose(result);
//
//   return result;
//}
