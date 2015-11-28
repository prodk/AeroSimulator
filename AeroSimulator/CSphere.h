#ifndef AERO_SIMULATOR_CSPHERE_H
#define AERO_SIMULATOR_CSPHERE_H

#include "CParentGameObject.h"

#include "glm/vec3.hpp"
#include <vector>

namespace AeroSimulatorEngine
{
   class CSphere : public CParentGameObject
   {
   public:
      CSphere();
      virtual ~CSphere();

      virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      virtual void addCustomObjects(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods - override only some of them
      virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      virtual void updateTRMatrix(const glm::mat4x4 & trMatrix);
      virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix);

   private:
      void generateSphere();

      typedef std::shared_ptr<CCompositeGameObject> tLine;

   private:
      std::vector<glm::vec3> mVertices; ///@todo: use a map to avoid repeating vertices
      std::vector<GLuint> mIndices;
      std::vector<tLine> mNormals;
      std::shared_ptr<CGeometry> mLineGeometry;
      glm::mat4x4 mScaledTRMatrix;
   };
}

#endif // AERO_SIMULATOR_CSPHERE_H
