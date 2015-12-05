#ifndef AERO_SIMULATOR_CCOLOR_BILLBOARD_H
#define AERO_SIMULATOR_CCOLOR_BILLBOARD_H

#include "CLeafGameObject.h"

namespace AeroSimulatorEngine
{
   class CColorBillBoard : public CLeafGameObject
   {
   public:
      CColorBillBoard();
      virtual ~CColorBillBoard();

      virtual void setEnvironment();
      virtual void resetEnvironment();
   };
}
#endif // AERO_SIMULATOR_CCOLOR_BILLBOARD_H
