#ifndef AERO_SIMULATOR_CSPHERE_H
#define AERO_SIMULATOR_CSPHERE_H

//#include "CParentGameObject.h"
#include "CGameObject.h"

#include "glm/vec3.hpp"
#include "../AeroSimulator/include/glm/mat4x4.hpp"
#include <vector>

namespace AeroSimulatorEngine
{
   class CShader;
   class CGeometry;

   //class CSphere : public CParentGameObject
   class CSphere : public CGameObject
   {
   public:
      CSphere();
      virtual ~CSphere();

      //virtual void setShadersAndBuffers(std::shared_ptr<CShader>& pShader);

      //virtual void addCustomObjects(std::shared_ptr<CShader>& pShader);

      /// Composite-related methods - override only some of them
      //virtual void buildModelMatrix(const glm::mat4x4 & parentTRMatrix);
      ///@todo: probably add some timing arguments here
      //virtual void updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt);
      //virtual void updateModelMatrix(const glm::mat4x4 & rootModelMatrix = glm::mat4x4(1.0f));

      //virtual bool loadTexture(const char * fileName);
      //virtual bool loadNormalMapTexture(const char* filePath);
      //virtual bool loadAnimationTexture(const char* filePath);

      void createNonTexturedGeometry(); ///@todo: probably make this private again and move to constructor

      virtual void update(const float deltaTime);

   private:
      void generateNonTexutredSphere();
      void generateTangents();
      void addTangents(std::shared_ptr<CShader>& pShader);

      //typedef std::shared_ptr<CCompositeGameObject> tLine;

   private:
      std::vector<glm::vec3> mVertices; ///@todo: use a map to avoid repeating vertices
      //std::vector<GLuint> mIndices;

      /// Normals
      //std::vector<tLine> mNormalLine; // Lines depicting normals
      std::vector<std::shared_ptr<CGeometry> > mGeometryNormals;
      std::vector<glm::vec3> mDataNormals;

      /// Tangents
      //std::vector<tLine> mTangentLine; // Lines depicting tangents
      std::vector<std::shared_ptr<CGeometry> > mGeometryTangents;
      std::vector<glm::vec3> mDataTangents;

      //glm::mat4x4 mScaledTRMatrix;
   };
}

#endif // AERO_SIMULATOR_CSPHERE_H
