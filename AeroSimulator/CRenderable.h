// CRenderable.h - renderable class declaration

#ifndef AERO_SIMULATOR_CRENDERABLE_H
#define AERO_SIMULATOR_CRENDERABLE_H

#include "../AeroSimulator/include/glew.h"
#include "../AeroSimulator/include/wglew.h"
#include <gl/GL.h>
#include "../AeroSimulator/include/glext.h"

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

      void setGeometry(CGeometry* pGeometry) { mGeometry.reset(pGeometry); }
      CGeometry* getGeometry() const { return mGeometry.get(); }

      void setShader(CShader* pShader) { mShader.reset(pShader); }
      CShader* getShader() const { return mShader.get(); }

      GLuint getVboId() const { return mVboId; }
      GLuint getIboId() const { return mIboId; }

      void setModelMatrix(const glm::mat4& m) { mModelMatrix = m; }
      glm::mat4 getModelMatrix() const { return mModelMatrix; }

      void setParentModelMatrix(const glm::mat4& m) { mParentModelMatrix = m; }
      glm::mat4 getParentModelMatrix() const { return mParentModelMatrix; }

      void setMvpMatrix(const glm::mat4& m) { mMvpMatrix = m; }
      glm::mat4 getMvpMatrix() const { return mMvpMatrix; }

      CTexture* getTexture() const { return mTexture.get(); }

      virtual bool loadTexture(const char* filePath);

   protected:
      std::shared_ptr<CGeometry> mGeometry;
      std::shared_ptr<CShader> mShader;
      std::shared_ptr<CTexture> mTexture;
      glm::mat4 mModelMatrix;
      GLuint mVboId;
      GLuint mIboId;
      glm::mat4 mParentModelMatrix; // Matrix of the root object in the tree
      glm::mat4 mMvpMatrix;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERABLE_H
