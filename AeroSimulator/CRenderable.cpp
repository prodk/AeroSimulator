#include "CRenderable.h"
#include "CGeometry.h"
#include "../src/shaders/CShader.h"
#include "CTexture.h"
//#include "CLog.h"
//#include "CCommonMath.h"

//#include "glm/gtc/matrix_transform.hpp"
#include <cassert>

using namespace AeroSimulatorEngine;

CRenderable::CRenderable()
   : mGeometry()
   , mShader()
   , mTextures(eTextures::LAST_TEXTURE)
   , mFlags()
   , m1DParams()
   , mVector2Params()
   , mVector3Params()
   , mVector4Params()
   , mMatrix3Params()
   , mMatrix4Params()
{
}

//CRenderable::CRenderable(GLfloat* pVertices, GLuint* pIndices, std::shared_ptr<CShader>& pShader, const char * mainTextureFilePath)
//   : mGeometry()
//   , mShader(pShader)
//   , mTextures(eTextures::LAST_TEXTURE)
//   , mFlags()
//   , m1DParams()
//   , mVector2Params()
//   , mVector3Params()
//   , mVector4Params()
//   , mMatrix3Params()
//   , mMatrix4Params()
//{
//   setGeometry(pVertices, pIndices);
//   createTexture(MAIN_TEXTURE);
//   if ((0 != mainTextureFilePath) && loadTexture(MAIN_TEXTURE, mainTextureFilePath, DDS))
//   {
//      LOG(("* CRenderable: texture loaded, filePath: ", mainTextureFilePath));
//   }
//
//   assert(mShader);
//}

CRenderable::~CRenderable()
{
   mGeometry.reset();
   mShader.reset();

   for (std::size_t id = 0; id < mTextures.size(); ++id)
   {
      mTextures[id].reset();
   }
   //mTexture.reset();
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
   auto result = false;

   auto iter = mFlags.find(id);
   if (mFlags.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      LOG("* CRenderable: Failed to find a flag: ", id);
   }

   return result;
}

float CRenderable::get1DParam(const int id) const
{
   auto result = 0.0f;

   auto iter = m1DParams.find(id);
   if (m1DParams.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      LOG("* CRenderable: Failed to find 1D param: ", id);
   }

   return result;
}

glm::vec2 CRenderable::getVector2Param(const int id) const
{
   return findValueInMap<glm::vec2>(mVector2Params, id, " vector 2D, key ");

   ///@todo: remove commented code when tested
   /*auto result = glm::vec2();

   auto iter = mVector2Params.find(id);
   if (mVector2Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      LOG("* CRenderable: Failed to find vector 2D: ", id);
   }

   return result;*/
}

glm::vec3 CRenderable::getVector3Param(const int id) const
{
   return findValueInMap<glm::vec3>(mVector3Params, id, " vector 3D, key ");

   ///@todo: remove commented code when tested
   /*auto result = glm::vec3();

   auto iter = mVector3Params.find(id);
   if (mVector3Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      LOG("* CRenderable: Failed to find vector 3D: ", id);
   }

   return result;*/
}

glm::vec4 CRenderable::getVector4Param(const int id) const
{
   return findValueInMap<glm::vec4>(mVector4Params, id, " vector 4D, key ");

   ///@todo: remove when tested
   /*auto result = glm::vec4();

   auto iter = mVector4Params.find(id);
   if (mVector4Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      LOG("* CRenderable: Failed to find vector 4D: ", id);
   }

   return result;*/
}

glm::mat3 CRenderable::getMatrix3Param(const int id) const
{
   return findValueInMap<glm::mat3>(mMatrix3Params, id, " matrix 3x3, key ");

   ///@todo: remove when tested
  /* auto result = glm::mat3();

   auto iter = mMatrix3Params.find(id);
   if (mMatrix3Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      LOG("* CRenderable: Failed to find matrix 3D: ", id);
   }

   return result;*/
}

glm::mat4 CRenderable::getMatrix4Param(const int id) const
{
   return findValueInMap<glm::mat4>(mMatrix4Params, id, " matrix 4x4, key ");

   ///@todo: remove when tested
   /*auto result = glm::mat4();

   auto iter = mMatrix4Params.find(id);
   if (mMatrix4Params.end() != iter)
   {
      result = iter->second;
   }
   else
   {
      LOG("* CRenderable: Failed to find matrix 4D: ", id);
   }

   return result;*/
}

bool CRenderable::loadTexture(const int id, const char * filePath, const int fmt)
{
   bool result = false;
   if (id < static_cast<int>(mTextures.size()) && mTextures[id])
   {
      switch (fmt)
      {
      case eTextureFileFormat::BMP:
         result = (0 != mTextures[id]->loadBmpTexture(filePath));
         break;
      default:
         result = (0 != mTextures[id]->loadDDSTexture(filePath));
         break;
      }

      if (result && (mTextures[id]->getWidth() != mTextures[id]->getHeight()))
      {
         glGenerateTextureMipmap(mTextures[id]->getId());
         CHECKGL("* CRenderable: glGenerateTextureMipmap() failed with error ");
         LOG("* CRenderable:: generating mipmaps for non-square texture, height: ", mTextures[id]->getHeight());
      }
   }

   return result;
}

void CRenderable::setShader(std::shared_ptr<CShader>& pShader)
{
   if (pShader)
   {
      mShader = pShader;
   }
}

void CRenderable::setGeometry(GLfloat* vertices, GLuint* indices)
{
   if (vertices && indices)
   {
      const std::size_t numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
      const std::size_t numOfIndices = sizeof(indices) / sizeof(indices[0]);

      if ((numOfVertices >0) && (numOfIndices > 0))
      {
         mGeometry.reset(new CGeometry());
         assert(mGeometry);

         mGeometry->setVertexBuffer(vertices);
         mGeometry->setNumOfVertices(numOfVertices);

         mGeometry->setIndexBuffer(indices);
         mGeometry->setNumOfIndices(numOfIndices);

         ///@todo: add these to the function args, probably through pOwner
         mGeometry->setNumOfElementsPerVertex(2);
         mGeometry->setVertexStride(4); // 2 coords + 2 tex coords

         // Can be called only when the correct geometry has been setup
         setupVbo();
      }
   }
}

void CRenderable::setupVbo()
{
   if (mGeometry) ///@todo: probably check that it has not been setup yet
   {
      // VBO
      GLuint vboid = 0;
      glGenBuffers(1, &vboid);
      CHECKGL("* CRenderable: glGenBuffers(1, &vboId) failed ");

      set1DParam(VBO0_ID, static_cast<float>(vboid));

      glBindBuffer(GL_ARRAY_BUFFER, vboid);
      CHECKGL("* CRenderable: glBindBuffer(GL_ARRAY_BUFFER, vboid) failed ");

      GLuint* data = static_cast<GLuint*>(mGeometry->getVertexBuffer());
      glBufferData(GL_ARRAY_BUFFER, mGeometry->getNumOfVertices()* sizeof(GLuint), data, GL_STATIC_DRAW);
      CHECKGL("* CRenderable: glBufferData(GL_ARRAY_BUFFER) failed ");

      // Index buffer
      GLuint iboid = 0;
      glGenBuffers(1, &iboid);
      CHECKGL("* CRenderable: glGenBuffers(1, &iboid) failed ");

      set1DParam(IBO0_ID, static_cast<float>(iboid));

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboid);
      CHECKGL("* CRenderable: glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboid) failed ");

      GLuint* indices = (GLuint*)mGeometry->getIndexBuffer();
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, mGeometry->getNumOfIndices()* sizeof(GLuint), indices, GL_STATIC_DRAW);
      CHECKGL("* CRenderable: glBufferData(GL_ELEMENT_ARRAY_BUFFER) failed ");

      // Reset VBOs
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }
}

void CRenderable::createTexture(const int id)
{
   if (id < static_cast<int>(mTextures.size()))
   {
      mTextures[id].reset(new CTexture());
      assert(mTextures[id]);
      LOG("* CRenderable: Texture created, its type is ", id);
   }
}

void CRenderable::createAndLoadTexture(const int id, const char * filePath, const int fmt)
{
   createTexture(id);
   if ((0 != filePath) && loadTexture(id, filePath, fmt))
   {
      LOG(("* CRenderable: texture loaded, filePath: ", filePath));
   }
}

//bool CRenderable::loadNormalMapTexture(const char * filePath)
//{
//   return false;
//}
//
//bool CRenderable::loadAnimationTexture(const char * filePath)
//{
//   return false;
//}

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
