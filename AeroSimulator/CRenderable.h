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

   // CRenderable groups the geometry and its appearance in one entity
   class CRenderable
   {
   public:
      CRenderable();
      ~CRenderable();

      void setGeometry(CGeometry* pGeometry) { mGeometry.reset(pGeometry); }
      CGeometry* getGeometry() const { return mGeometry.get(); }

      void setShader(CShader* pShader) { mShader.reset(pShader); }
      CShader* getShader() const { return mShader.get(); }

      GLuint getVboId() const { return mVboId; }
      GLuint getIboId() const { return mIboId; }

   protected:
      /*CGeometry* mGeometry;
      CShader* mShader;*/
      std::shared_ptr<CGeometry> mGeometry;
      std::shared_ptr<CShader> mShader;
      glm::mat4 mModelMatrix;
      GLuint mVboId;
      GLuint mIboId;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CRENDERABLE_H
