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

      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));
   };
}
#endif // AERO_SIMULATOR_CCOLOR_BILLBOARD_H
