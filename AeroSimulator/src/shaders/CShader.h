// CShader.h - declaration of a base shader class

#ifndef AERO_SIMULATOR_CSHADER_H
#define AERO_SIMULATOR_CSHADER_H

#include "glew.h"
#include <gl/GL.h>
#include <glext.h>

#include <string>

namespace AeroSimulatorEngine
{
   class CRenderable;

   class CShader
   {
   public:
      CShader();
      virtual ~CShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);
      bool isLinked() const { return mIsLinked; }

      int getElementsPerVertex() const { return mElementsPerVertex; }
      int getStride() const { mStride; }

      void setElementsPerVertex(const int elem) { mElementsPerVertex = elem; }
      void setStride(const int stride) { mStride = stride; }

      enum eElementsPerVertex 
      {
         eFboElements = 2,
         eTextureElements = 3
      };

      enum eStride
      {
         eColorStride = 3,    // 3 coords
         eFboStride = 4,      // 2 coords + 2 tex coords
         eTextureStride = 5   // 3 coords + 2 tex coords
      };

   protected:
      GLint loadShader(GLuint id, const std::string& shaderCode);
      std::string readShader(const char* filePath);

   protected:
      GLuint mVertexShaderId;
      GLuint mFragmentShaderId;
      GLint mProgramId;
      std::string mVertexShaderCode;
      std::string mFragmentShaderCode;
      bool mIsLinked;
      int mElementsPerVertex;
      int mStride;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CSHADER_H
