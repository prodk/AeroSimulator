#ifndef AERO_SIMULATOR_CLAND_H
#define AERO_SIMULATOR_CLAND_H

#include "CParentGameObject.h"

namespace AeroSimulatorEngine
{
   class CBoundingBox;

   class CLand : public CParentGameObject
   {
   public:
      CLand();
      virtual ~CLand();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader); /// Requires a valid render context

      bool loadTexture(const char * fileName);

      /// Composite-related methods - override only some of them
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));

      void setBoundingBox(std::shared_ptr<CShader>& pShader, const glm::vec3& size, const glm::vec4& color);
      const CBoundingBox* getBoundingBox() const;

      void setNumOfTiles(const GLint x, const GLint y)
      {
         if (x > 0)
            mNumOfTilesX = x;
         if (y > 0)
            mNumOfTilesY = y;
      }

   private:
      glm::mat4x4 mScaledTRMatrix;
      std::shared_ptr<CBoundingBox> mBoundingBox;
      GLint mNumOfTilesX;
      GLint mNumOfTilesY;
   };
} //namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLAND_H
