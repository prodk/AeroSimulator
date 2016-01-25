#ifndef AERO_SIMULATOR_CAXES_FRAME_H
#define AERO_SIMULATOR_CAXES_FRAME_H

//#include "CParentGameObject.h"
#include "CGameObject.h"

namespace AeroSimulatorEngine
{
   class CShader;
   class CGeometry;

   class CAxesFrame : public CGameObject// CParentGameObject
   {
   public:
      CAxesFrame();
      virtual ~CAxesFrame();

      void buildModel(std::shared_ptr<CShader>& pShader);

   private:
      std::shared_ptr<CGeometry> mLineGeometry;
      /*std::shared_ptr<CCompositeGameObject> mAxisX;
      std::shared_ptr<CCompositeGameObject> mAxisY;
      std::shared_ptr<CCompositeGameObject> mAxisZ;*/
   };
} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CAXES_FRAME_H
