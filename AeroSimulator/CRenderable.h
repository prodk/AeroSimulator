// CRenderable.h - renderable class declaration

#ifndef AERO_SIMULATOR_CRENDERABLE_H
#define AERO_SIMULATOR_CRENDERABLE_H

#include "../AeroSimulator/include/glew.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
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

      /// General
      virtual void setEnvironment();
      virtual void resetEnvironment();
      bool canBeRendered() const { return (0 != mGeometry) && (0 != mShader); }

      /// Textures
      virtual bool loadTexture(const char* filePath);
      CTexture* getTexture() const { return mTexture.get(); }

      /// Geometry
      void setGeometry(std::shared_ptr<CGeometry>& pGeometry) { mGeometry = pGeometry; }
      CGeometry* getGeometry() const { return mGeometry.get(); }

      /// Shaders
      void setShader(CShader* pShader) { mShader.reset(pShader); }
      CShader* getShader() const { return mShader.get(); }

      GLuint getVboId() const { return mVboId; }
      GLuint getIboId() const { return mIboId; }

      void setTexture(std::shared_ptr<CTexture>& pTexture) { mTexture = pTexture; }

      /// Matrices
      void setModelMatrix(const glm::mat4& m) { mModelMatrix = m; }
      glm::mat4 getModelMatrix() const { return mModelMatrix; }

      void setMvpMatrix(const glm::mat4& m) { mMvpMatrix = m; }
      glm::mat4 getMvpMatrix() const { return mMvpMatrix; }

      void setViewMatrix(const glm::mat4& m) { mViewMatrix = m; }
      glm::mat4 getViewMatrix() const { return mViewMatrix; }

      glm::mat3 getNormalMatrix() const;

      void setEyePos(const glm::vec3& eyePos) { mEyePos = eyePos; }
      glm::vec3 getEyePos() const { return mEyePos; }

      /// Billboards
      void setRightVector(const glm::vec3 & right) { mRightVector = right; }
      glm::vec3 getRightVector() const { return mRightVector; }
      void setUpVector(const glm::vec3 & up) { mUpVector = up; }
      glm::vec3 getUpVector() const { return mUpVector; }

      void setBillboardWidth(const float width) { mBillboardWidth = width; }
      float getBillboardWidth() const { return mBillboardWidth; }
      void setBillboardHeight(const float height) { mBillboardHeight = height; }
      float getBillboardHeight() const { return mBillboardHeight; }

      void setDrawWithLines(bool flag) { mDrawWithLines = flag; }
      bool getDrawWithLines() const { return mDrawWithLines; }

      // Uniform color renderables-specific
      void setColor(const glm::vec4& color) { mColor = color; }
      glm::vec4 getColor() const { return mColor; }

      // Line-specific
      void setLineWidth(const float width) { mLineWidth = width; }
      float getLineWidth() const { return mLineWidth; }

      void setHealthValue(float shift) { mHealthValue = shift; }
      float getHealthValue() const { return mHealthValue; }

   protected:
      std::shared_ptr<CGeometry> mGeometry;
      std::shared_ptr<CShader> mShader;
      glm::mat4 mModelMatrix;
      glm::mat4 mMvpMatrix;
      std::shared_ptr<CTexture> mTexture;
      GLuint mVboId;
      GLuint mIboId;

      ///@todo: probably create a separate renderable for a billboard
      /// These are used only fro billboards
      glm::vec3 mRightVector;
      glm::vec3 mUpVector;
      float mBillboardWidth;
      float mBillboardHeight;

      bool mDrawWithLines;
      glm::vec4 mColor; // Only for color shader
      float mLineWidth; // Only for debug mode

      ///@todo: change the architecture such that it is not required to add all
      ///@todo these variables to CRenderer. They should be added to those objects where they are needed!
      float mHealthValue; // Shift of the healthbar foreground relative to the background
      glm::mat4 mViewMatrix;
      glm::vec3 mEyePos;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERABLE_H
