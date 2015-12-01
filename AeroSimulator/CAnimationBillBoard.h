#ifndef AERO_SIMULATOR_CANIMATION_BILLBOARD_H
#define AERO_SIMULATOR_CANIMATION_BILLBOARD_H

#include "CBillBoard.h"

namespace AeroSimulatorEngine
{
   class CAnimationBillBoard : public CBillBoard
   {
   public:
      CAnimationBillBoard();
      virtual ~CAnimationBillBoard();

      virtual bool loadTexture(const char * fileName);
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CANIMATION_BILLBOARD_H
