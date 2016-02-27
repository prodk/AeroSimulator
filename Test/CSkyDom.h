#ifndef AERO_SIMULATOR_CSKYDOM_H
#define AERO_SIMULATOR_CSKYDOM_H

#include "../AeroSimulator/CGameObject.h"

#include <string>

namespace AeroSimulatorEngine
{
   class CShader;
   class CRenderable;
   struct SRenderableData;

   class CSkyDom : public CGameObject
   {
   public:
      CSkyDom(const int id,
              const int type,
              SRenderableData& data);

      virtual ~CSkyDom();

   private:
      void addRenderableComponent(SRenderableData& data);
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CSKYDOM_H
