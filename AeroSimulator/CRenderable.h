// CRenderable.h - renderable class declaration

#ifndef AERO_SIMULATOR_CRENDERABLE_H
#define AERO_SIMULATOR_CRENDERABLE_H

#include "../AeroSimulator/include/glew.h"
#include "glm/mat4x4.hpp"
#include <memory>

namespace AeroSimulatorEngine
{
   // Fwd declarations should be in the same namespace
   class CShader;
   class CGeometry;
   class CTexture;

   // CRenderable groups the geometry and its appearance in one entity
   class CRenderable
   {
   public:
      CRenderable();
      virtual ~CRenderable();

      void setGeometry(std::shared_ptr<CGeometry>& pGeometry) { mGeometry = pGeometry; }
      CGeometry* getGeometry() const { return mGeometry.get(); }

      void setShader(CShader* pShader) { mShader.reset(pShader); }
      CShader* getShader() const { return mShader.get(); }

      GLuint getVboId() const { return mVboId; }
      GLuint getIboId() const { return mIboId; }

      void setModelMatrix(const glm::mat4& m) { mModelMatrix = m; }
      glm::mat4 getModelMatrix() const { return mModelMatrix; }

      void setMvpMatrix(const glm::mat4& m) { mMvpMatrix = m; }
      glm::mat4 getMvpMatrix() const { return mMvpMatrix; }

      CTexture* getTexture() const { return mTexture.get(); }

      bool canBeRendered() const { return (0 != mGeometry) && (0 != mShader); }

      virtual bool loadTexture(const char* filePath);

   protected:
      std::shared_ptr<CGeometry> mGeometry;
      std::shared_ptr<CShader> mShader;
      glm::mat4 mModelMatrix;
      glm::mat4 mMvpMatrix;
      std::shared_ptr<CTexture> mTexture;
      GLuint mVboId;
      GLuint mIboId;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERABLE_H
