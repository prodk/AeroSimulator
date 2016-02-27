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
   enum eShaderMatrix4Params { MODEL_MATRIX, VIEW_MATRIX, PROJECTION_MATRIX};
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

   struct SGeometryData
   {
      SGeometryData(GLfloat* vertices, int numVertices, GLuint* indices, int numIndices);

      GLfloat* mVertices;
      int mNumVertices;
      GLuint* mIndices;
      int mNumIndices;
   };

   // CRenderable groups the geometry and its appearance in one entity
   class CRenderable
   {
   public:
      CRenderable();
      ~CRenderable();

      /// General
      bool canBeRendered() const { return (0 != mGeometry) && (0 != mShader); }
      void setEnvironment();
      void resetEnvironment();

      /// Geometry
      CGeometry* getGeometry() const { return mGeometry.get(); }
      void setGeometry(const SGeometryData& data);

      /// Shader
      void setShader(std::shared_ptr<CShader>& pShader);
      CShader* getShader() const { return mShader.get(); }

      /// Textures
      bool loadTexture(const int id, const char* filePath, const int fmt);
      void createTexture(const int id);
      void createAndLoadTexture(const int id, const char* filePath, const int fmt);
      CTexture* getTexture(const int id) const;

      ///@todo: probably make these inlined later
      // Setters for shader params
      void setFlag(const int id, const bool value);
      void set1DParam(const int id, const float value);
      void setVector2Param(const int id, const glm::vec2& value);
      void setVector3Param(const int id, const glm::vec3& value);
      void setVector4Param(const int id, const glm::vec4& value);
      void setMatrix3Param(const int id, const glm::mat3& value);
      void setMatrix4Param(const int id, const glm::mat4& value);

      ///@todo: probably make these inlined later
      // Getters for shader params
      bool getFlag(const int id) const;
      float get1DParam(const int id) const;
      glm::vec2 getVector2Param(const int id) const;
      glm::vec3 getVector3Param(const int id) const;
      glm::vec4 getVector4Param(const int id) const;
      glm::mat3 getMatrix3Param(const int id) const;
      glm::mat4 getMatrix4Param(const int id) const;

      ///@todo: remove this commented code

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

      //void setTransparent(const bool flag) { mIsTransparent = flag; }
      //bool isTransparent() const { return mIsTransparent; }

   private:
      template <typename T>
      T findValueInMap(const std::map<int, T>& sourceMap, const int key, const char* msg) const;

      void setupVbo();

   private:
      std::unique_ptr<CGeometry> mGeometry;
      std::shared_ptr<CShader> mShader;
      std::vector<std::unique_ptr<CTexture> > mTextures;

      std::map<int, bool> mFlags;              // Flags, e.g. drawWithLines, isVisible, repeat texture, isTransparent
      std::map<int, float> m1DParams;          // 1D parameters ints and floats, ints are saved as floats
      std::map<int, glm::vec2> mVector2Params; // 2D vectors
      std::map<int, glm::vec3> mVector3Params; // 3D vectors
      std::map<int, glm::vec4> mVector4Params; // 4D vectors
      std::map<int, glm::mat3> mMatrix3Params; // 3x3 matrices
      std::map<int, glm::mat4> mMatrix4Params; // 4x4 matrices

      ///@todo: add an enum with texture IDs {USUAL, NORAML_MAP, ANIMATION}, then add textures to a vector or a map, see shaders in CGame

      /// These are used only fro billboards
      //glm::vec3 mRightVector;
      //glm::vec3 mUpVector;
      //float mBillboardWidth;
      //float mBillboardHeight;

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
