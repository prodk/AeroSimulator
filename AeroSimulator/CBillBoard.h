#ifndef AERO_SIMULATOR_CBILLBOARD_H
#define AERO_SIMULATOR_CBILLBOARD_H

#include "CLeafCompositeGameObject.h"

namespace AeroSimulatorEngine
{
   class CBillBoard : public CLeafCompositeGameObject
   {
   public:
      CBillBoard();
      virtual ~CBillBoard();

      virtual void setEnvironment();
      virtual void resetEnvironment();

      bool loadTexture(const char * fileName);
   };
}

#endif // AERO_SIMULATOR_CBILLBOARD_H
