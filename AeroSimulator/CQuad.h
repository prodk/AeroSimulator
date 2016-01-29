#ifndef AERO_SIMULATOR_CQUAD_H
#define AERO_SIMULATOR_CQUAD_H

#include "CGameObject.h"
#include "../AeroSimulator/include/glew.h"

namespace AeroSimulatorEngine
{
   class CRenderable;
   class CShader;

   class CQuad : public CGameObject
   {
   public:
      CQuad();
      virtual ~CQuad();

      // Needed because we cannot call the GL stuff in the constructor without a valid GL context
      void prepareRenderable(std::shared_ptr<CShader>& pShader,
                             int textureId,
                             GLuint openGlId = 0,
                             GLenum textureUnit = GL_TEXTURE0);
      void setShader(std::shared_ptr<CShader>& pShader);
      void setTextureId(const int textureId, const GLuint id);

      void setTextureUnit(const GLenum unit);

      CRenderable& getRenderable();

   private:
      bool mIsInitialized;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CQUAD_H
