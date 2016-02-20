#ifndef AERO_SIMULATOR_CSKYDOM_H
#define AERO_SIMULATOR_CSKYDOM_H

#include "../AeroSimulator/CGameObject.h"

namespace AeroSimulatorEngine
{
   class CShader;
   class CRenderable;

   struct SSphereParams
   {
      SSphereParams(float radius, std::size_t numOfCircles, std::size_t numOfSegments, float maxInclination, float maxAzimuth);

      float mRradius;
      std::size_t mNumOfCircles;
      std::size_t mNumOfSegments;
      float mMaxInclination;
      float mMaxAzimuth;
   };

   class CSkyDom : public CGameObject
   {
   public:
      ///@todo: add scale as an argument
      CSkyDom(const int id,
              const int type,
              std::shared_ptr<CShader>& pShader,
              const char * textureFilePath,
              const SSphereParams& generateParams);

      virtual ~CSkyDom();

   private:
      ///@todo: make addComponent virtual such that no need to declare them in every class
      void addRenderableComponent(std::shared_ptr<CShader>& pShader, const char * textureFilePath, const SSphereParams& generateParams);
      void addTransformComponent();
      void addMovementComponent();

      CRenderable & CSkyDom::getRenderable();
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CSKYDOM_H
