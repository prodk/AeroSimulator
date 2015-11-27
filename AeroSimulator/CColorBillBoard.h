#ifndef AERO_SIMULATOR_CCOLOR_BILLBOARD_H
#define AERO_SIMULATOR_CCOLOR_BILLBOARD_H

#include "CLeafCompositeGameObject.h"

namespace AeroSimulatorEngine
{
   class CColorBillBoard : public CLeafCompositeGameObject
   {
   public:
      CColorBillBoard();
      virtual ~CColorBillBoard();

      virtual void setEnvironment();
      virtual void resetEnvironment();
   };
}
#endif // AERO_SIMULATOR_CCOLOR_BILLBOARD_H
