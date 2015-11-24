#ifndef AERO_SIMULATOR_CLINE_H
#define AERO_SIMULATOR_CLINE_H

#include "CCompositeGameObject.h"
#include <memory>

namespace AeroSimulatorEngine
{
   class CLine : public CCompositeGameObject
   {
   public:
      CLine();
      CLine(std::shared_ptr<float>& vertices);
      virtual ~CLine();

   private:
      std::shared_ptr<float> mVertices; ///@todo: may be redundant, could be passed directly in setGeometry()
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CLINE_H
