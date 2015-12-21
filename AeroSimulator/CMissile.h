#ifndef AERO_SIMULATOR_CMISSILE_H
#define AERO_SIMULATOR_CMISSILE_H

#include "CCube.h"

namespace AeroSimulatorEngine
{
   class CMissile : public CCube
   {
   public:
      CMissile();
      virtual ~CMissile();

      bool isDetached() const { return mIsDetached; }
      void setDetached(const bool detach) { mIsDetached = detach; }

   private:
      bool mIsDetached;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CMISSILE_H
