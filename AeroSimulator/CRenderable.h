// CRenderable.h - renderable class declaration

#ifndef AERO_SIMULATOR_CRENDERABLE_H
#define AERO_SIMULATOR_CRENDERABLE_H

#include "CLog.h"
#include "../AeroSimulator/include/glew.h"
#include <memory>
#include <vector>
#include <map>

#include "../AeroSimulator/include/glm/vec2.hpp"
#include "../AeroSimulator/include/glm/vec3.hpp"
#include "../AeroSimulator/include/glm/mat3x3.hpp"
#include "../AeroSimulator/include/glm/mat4x4.hpp"

namespace
{
   // Global constants for setting rendering flags and shader uniforms.
   enum eShaderFlags { REPEAT_TEXTURE, DRAW_LINES, IS_VISIBLE, IS_TRANSPARENT };
   enum eShader1DParams { VBO0_ID, IBO0_ID, BILLBOARD_WIDTH, BILLBOARD_HEIGHT, LINE_WIDTH, HEALTH, TEXTURE_UNIT };
   enum eShaderVector2Params { CURRENT_FRAME, FRAME_SIZE, NUM_OF_FRAMES };
   enum eShaderVector3Params { EYE_POSITION, RIGHT_VECTOR, UP_VECTOR };
   enum eShaderVector4Params { COLOR };
   enum eShaderMatrix3Params { NORMAL_MATRIX };
   enum eShaderMatrix4Params { MODEL_MATRIX, MVP_MATRIX, VIEW_MATRIX };
   enum eEnvironment { DEPTH };
   enum eTextures { MAIN_TEXTURE, NORMAL_MAP_TEXTURE, ANIMATION_TEXTURE, LAST_TEXTURE };
   enum eTextureFileFormat { DDS, BMP };
}

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
      //CRenderable(GLfloat* pVertices, GLuint* pIndices, std::shared_ptr<CShader>& pShader, const char* mainTextureFilePath = 0);
      virtual ~CRenderable();

      /// General
      virtual void setEnvironment();
      virtual void resetEnvironment();
      bool canBeRendered() const { return (0 != mGeometry) && (0 != mShader); }

      // Setters for shader params
      void setFlag(const int id, const bool value);
      void set1DParam(const int id, const float value);
      void setVector2Param(const int id, const glm::vec2& value);
      void setVector3Param(const int id, const glm::vec3& value);
      void setVector4Param(const int id, const glm::vec4& value);
      void setMatrix3Param(const int id, const glm::mat3& value);
      void setMatrix4Param(const int id, const glm::mat4& value);

      // Getters for shader params
      bool getFlag(const int id) const;
      float get1DParam(const int id) const;
      glm::vec2 getVector2Param(const int id) const;
      glm::vec3 getVector3Param(const int id) const;
      glm::vec4 getVector4Param(const int id) const;
      glm::mat3 getMatrix3Param(const int id) const;
      glm::mat4 getMatrix4Param(const int id) const;


      CGeometry* getGeometry() const { return mGeometry.get(); }
      CShader* getShader() const { return mShader.get(); }
      CTexture* getTexture() const { return mTextures[MAIN_TEXTURE].get(); }

      ///@todo: add numOfElements and stride to the function args, probably through pOwner
      void setGeometry(GLfloat* vertices, GLuint* indices);
      void setShader(std::shared_ptr<CShader>& pShader);
      bool loadTexture(const int id, const char* filePath, const int fmt);
      void createTexture(const int id);
      void createAndLoadTexture(const int id, const char* filePath, const int fmt);


      //bool loadNormalMapTexture(const char* filePath);
      //bool loadAnimationTexture(const char* filePath);
      //CTexture* getTexture() const { return mTexture.get(); }
      //CTexture* getNormalMapTexture() const { return mNormalMapTexture.get(); }
      //CTexture* getAnimationTexture() const { return mAnimationTexture.get(); }

      /// Geometry
      //void setGeometry(std::shared_ptr<CGeometry>& pGeometry) { mGeometry = pGeometry; }
      //CGeometry* getGeometry() const { return mGeometry.get(); }

      /// Shaders
      //void setShader(CShader* pShader) { mShader.reset(pShader); }
      //CShader* getShader() const { return mShader.get(); }

      //void setTexture(std::shared_ptr<CTexture>& pTexture) { mTexture = pTexture; }

      //void setRepeatTexture(const bool repeat) { mRepeatTexture = repeat; }
      //bool isRepeatTexture() const { return mRepeatTexture; }

      //GLuint getVboId() const { return mVboId; }
      //GLuint getIboId() const { return mIboId; }

      /// Matrices
      //void setModelMatrix(const glm::mat4& m) { mModelMatrix = m; }
      //glm::mat4 getModelMatrix() const { return mModelMatrix; }

      //void setMvpMatrix(const glm::mat4& m) { mMvpMatrix = m; }
      //glm::mat4 getMvpMatrix() const { return mMvpMatrix; }

      //void setViewMatrix(const glm::mat4& m) { mViewMatrix = m; }
      //glm::mat4 getViewMatrix() const { return mViewMatrix; }

      //glm::mat3 getNormalMatrix() const;

      //void setEyePos(const glm::vec3& eyePos) { mEyePos = eyePos; }
      //glm::vec3 getEyePos() const { return mEyePos; }

      /// Billboards
      //void setRightVector(const glm::vec3 & right) { mRightVector = right; }
      //glm::vec3 getRightVector() const { return mRightVector; }
      //void setUpVector(const glm::vec3 & up) { mUpVector = up; }
      //glm::vec3 getUpVector() const { return mUpVector; }

      //void setBillboardWidth(const float width) { mBillboardWidth = width; }
      //float getBillboardWidth() const { return mBillboardWidth; }
      //void setBillboardHeight(const float height) { mBillboardHeight = height; }
      //float getBillboardHeight() const { return mBillboardHeight; }

      //void setDrawWithLines(bool flag) { mDrawWithLines = flag; }
      //bool getDrawWithLines() const { return mDrawWithLines; }

      // Uniform color renderables-specific
      //void setColor(const glm::vec4& color) { mColor = color; }
      //glm::vec4 getColor() const { return mColor; }

      // Line-specific
      //void setLineWidth(const float width) { mLineWidth = width; }
      //float getLineWidth() const { return mLineWidth; }

      //void setHealthValue(float shift) { mHealthValue = shift; }
      //float getHealthValue() const { return mHealthValue; }

      // Animation-specific
      //void setCurrentFrame(const glm::vec2& frame) { mCurrentFrame = frame; }
      //glm::vec2 getCurrentFrame() const { return mCurrentFrame; }

      /*void setFrameSize(const glm::vec2& size)
      {
         mFrameSize = size;
         if (mFrameSize.length())
         {
            mNumOfFrames.x = 1.f / mFrameSize.x;
            mNumOfFrames.y = 1.f / mFrameSize.y;
         }
      }*/
      //glm::vec2 getFrameSize() const { return mFrameSize; }

      //bool isVisible() const { return mIsVisible; }

      //void setTransparent(const bool flag) { mIsTransparent = flag; }
      //bool isTransparent() const { return mIsTransparent; }

      //virtual void setVisible(const bool visible) { mIsVisible = visible; }

      //void setTextureUnit(const GLint unit) { mTextureUnit = unit; }
      //GLint getTextureUnit() const { return mTextureUnit; }

   private:
      template <typename T>
      T findValueInMap(const std::map<int, T>& sourceMap, const int key, const char* msg) const;

      void setupVbo();

   //protected:
   private:
      ///@todo: rearrange this params as well as in the initialization list
      std::unique_ptr<CGeometry> mGeometry;
      std::shared_ptr<CShader> mShader;
      std::vector<std::unique_ptr<CTexture> > mTextures;

      std::map<int, bool> mFlags;              // Flags, e.g. drawWithLines, isVisible, repeat texture, isTransparent
      std::map<int, float> m1DParams;          // 1D parameters ints and floats, ints are saved as floats
      std::map<int, glm::vec2> mVector2Params; // 2D vectors
      std::map<int, glm::vec3> mVector3Params; // 3D vectors
      std::map<int, glm::vec4> mVector4Params; // 4D vectors
      std::map<int, glm::mat3> mMatrix3Params; // 3x3 matrices
      std::map<int, glm::mat4> mMatrix4Params;// 4x4 matrices

      ///@todo: add an enum with texture IDs {USUAL, NORAML_MAP, ANIMATION}, then add textures to a vector or a map, see shaders in CGame
      /*std::shared_ptr<CTexture> mTexture;
      std::shared_ptr<CTexture> mNormalMapTexture;
      std::shared_ptr<CTexture> mAnimationTexture;*/


      //glm::mat4 mModelMatrix;
      //glm::mat4 mMvpMatrix;
      //GLuint mVboId;
      //GLuint mIboId;

      ///@todo: probably create a separate renderable for a billboard
      /// These are used only fro billboards
      //glm::vec3 mRightVector;
      //glm::vec3 mUpVector;
      //float mBillboardWidth;
      //float mBillboardHeight;

      //bool mDrawWithLines;
      //glm::vec4 mColor; // Only for color shader
      //float mLineWidth; // Only for debug mode

      ///@todo: change the architecture such that it is not required to add all
      ///@todo these variables to CRenderer. They should be added to those objects where they are needed!
      ///@todo: or create several types of renderables for each type of objects
      //float mHealthValue; // Shift of the healthbar foreground relative to the background
      //glm::mat4 mViewMatrix;
      //glm::vec3 mEyePos;

      // Animation-related
      //glm::vec2 mCurrentFrame;
      //glm::vec2 mFrameSize;
      //glm::vec2 mNumOfFrames;

      //bool mIsVisible;
      //bool mRepeatTexture;
      //bool mIsTransparent;
      //GLint mTextureUnit;

      // The maps contain flags and parameters used mainly in shaders as uniform values.
      // A shader gives the name of the parameter and gets its value
      // In this way we avoid hardcodig unnecessary values in every instance of this class.
   };

   template <typename T>
   T CRenderable::findValueInMap(const std::map<int, T>& sourceMap, const int key, const char* msg) const
   {
      T result = T();

      auto iter = sourceMap.find(key);
      if (sourceMap.end() != iter)
      {
         result = iter->second;
      }
      else
      {
         LOG("Failed to find ", msg, key);
      }

      return result;
   }
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERABLE_H
