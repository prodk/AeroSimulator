#ifndef AERO_SIMULATOR_CTEXTURE_SHADER_H
#define AERO_SIMULATOR_CTEXTURE_SHADER_H

#include "CShader.h"

#include "../include/glew.h"
#include <gl/GL.h>

#include <memory>

namespace AeroSimulatorEngine
{
   class CTexture;

   class CTextureShader : public CShader
   {
   public:
      CTextureShader();
      virtual ~CTextureShader();

      virtual void link();
      virtual void setup(CRenderable& renderable);

   protected:
      std::shared_ptr<CTexture> mTexture;
      GLint mPositionAttributeId;
      GLint mTexCoordAttributeId;
      GLint mMvpAttributeId; ///@todo: rename to uniform id
      GLint mSamplerUniformId;

      void setTexture(std::shared_ptr<CTexture>& pTexture) { mTexture = pTexture; }
      std::shared_ptr<CTexture> getTexture() const { return mTexture; }
   };

} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CTEXTURE_SHADER_H
