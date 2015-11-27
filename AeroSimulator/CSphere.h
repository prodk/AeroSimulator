#ifndef AERO_SIMULATOR_CSPHERE_H
#define AERO_SIMULATOR_CSPHERE_H

#include "CLeafCompositeGameObject.h"

#include "glm/vec3.hpp"
#include <vector>

namespace AeroSimulatorEngine
{
   class CSphere : public CLeafCompositeGameObject
   {
   public:
      CSphere();
      virtual ~CSphere();

   private:
      void generateSphere();

   private:
      std::vector<glm::vec3> mVertices;
      std::vector<GLuint> mIndices;
   };
}

#endif // AERO_SIMULATOR_CSPHERE_H
