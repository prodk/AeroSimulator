
#ifndef AERO_SIMULATOR_CCUBE_H
#define AERO_SIMULATOR_CCUBE_H

#include "CGameObject.h"

#include <memory>

namespace AeroSimulatorEngine
{
   class CSimpleShader;

   class CCube : public CGameObject
   {
   public:
      CCube();
      virtual ~CCube();

      /// These methods require a valid render context
      virtual void setupGeometry();
      virtual void setupVBO();

   //private:

      ///@todo: probably mGeometry and mShader are redundant;
      /*std::shared_ptr<CGeometry> mGeometry;
      std::shared_ptr<CSimpleShader> mShader;*/
   };

}

#endif // AERO_SIMULATOR_CCUBE_H
