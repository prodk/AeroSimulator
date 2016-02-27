#include "CRenderable.h"
#include "CGeometry.h"
#include "../src/shaders/CShader.h"
#include "CTexture.h"
#include <cassert>

using namespace AeroSimulatorEngine;

SGeometryData::SGeometryData(GLfloat* vertices, int numVertices, GLuint* indices, int numIndices)
   : mVertices(vertices)
   , mNumVertices(numVertices)
   , mIndices(indices)
   , mNumIndices(numIndices)
{
}

SRenderableData::SRenderableData(std::shared_ptr<CShader>& pShader, const SSphereParams * sphereParams, const std::string& textureFilePath)
   : mShader(pShader)
   , mSphereParams(sphereParams)
   , mTextureFilePath(textureFilePath)
{
}

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
   /// By default it is visible
   setFlag(eShaderFlags::IS_VISIBLE, true);
}

CRenderable::~CRenderable()
{
   mGeometry.reset();
   mShader.reset();

   for (std::size_t id = 0; id < mTextures.size(); ++id)
   {
      mTextures[id].reset();
   }
}

void CRenderable::resetEnvironment()
{
}

void CRenderable::setEnvironment()
{
}

void CRenderable::setGeometry(const SGeometryData& data)
{
   if (data.mVertices && data.mIndices && (data.mNumVertices > 0) && (data.mNumIndices > 0))
   {
      mGeometry.reset(new CGeometry());
      assert(mGeometry);

      mGeometry->setVertexBuffer((void*)data.mVertices);
      mGeometry->setNumOfVertices(data.mNumVertices);

      mGeometry->setIndexBuffer((void*)data.mIndices);
      mGeometry->setNumOfIndices(data.mNumIndices);

      // Can be called only when the correct geometry has been setup
      setupVbo();
   }
}

void CRenderable::setShader(std::shared_ptr<CShader>& pShader)
{
   if (pShader)
   {
      mShader = pShader;
      if (!mShader->isLinked())
      {
         mShader->link();
      }
   }
   else
   {
      LOG("* CRenderable::setShader(): error invalid shader!");
   }
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
         LOG("* CRenderable:: generating mipmaps for non-square texture, height: ", (unsigned int)mTextures[id]->getHeight());
      }
   }

   return result;
}

void CRenderable::createTexture(const int id)
{
   if (id < static_cast<int>(mTextures.size()))
   {
      mTextures[id].reset(new CTexture());
      assert(mTextures[id]);

      set1DParam(TEXTURE_UNIT, GL_TEXTURE0);
      LOG("* CRenderable: Texture created, its type is ", (unsigned int)id);
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

CTexture * CRenderable::getTexture(const int id) const
{
   return mTextures[id].get();
}

void CRenderable::setFlag(const int id, const bool value)
{
   mFlags[id] = value;
}

void CRenderable::set1DParam(const int id, const float value)
{
   m1DParams[id] = value;
}

void CRenderable::setVector2Param(const int id, const glm::vec2& value)
{
   mVector2Params[id] = value;
}

void CRenderable::setVector3Param(const int id, const glm::vec3& value)
{
   mVector3Params[id] = value;
}

void CRenderable::setVector4Param(const int id, const glm::vec4 & value)
{
   mVector4Params[id] = value;
}

void CRenderable::setMatrix3Param(const int id, const glm::mat3 & value)
{
   mMatrix3Params[id] = value;
}

void CRenderable::setMatrix4Param(const int id, const glm::mat4& value)
{
   mMatrix4Params[id] = value;
}

bool CRenderable::getFlag(const int id) const
{
   auto result = false;

   auto iter = mFlags.find(id);
   if (mFlags.end() != iter)
   {
      result = iter->second;
   }
   /*else
   {
      LOG("* CRenderable: Failed to find a flag: ", id);
   }*/

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
      LOG("* CRenderable: Failed to find 1D param: ", (unsigned int)id);
   }

   return result;
}

glm::vec2 CRenderable::getVector2Param(const int id) const
{
   return findValueInMap<glm::vec2>(mVector2Params, id, " vector 2D, key ");
}

glm::vec3 CRenderable::getVector3Param(const int id) const
{
   return findValueInMap<glm::vec3>(mVector3Params, id, " vector 3D, key ");
}

glm::vec4 CRenderable::getVector4Param(const int id) const
{
   return findValueInMap<glm::vec4>(mVector4Params, id, " vector 4D, key ");
}

glm::mat3 CRenderable::getMatrix3Param(const int id) const
{
   return findValueInMap<glm::mat3>(mMatrix3Params, id, " matrix 3x3, key ");
}

glm::mat4 CRenderable::getMatrix4Param(const int id) const
{
   return findValueInMap<glm::mat4>(mMatrix4Params, id, " matrix 4x4, key ");
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

      GLfloat* data = static_cast<GLfloat*>(mGeometry->getVertexBuffer());
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
