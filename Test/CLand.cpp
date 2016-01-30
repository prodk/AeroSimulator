#include "CLand.h"
#include "../CGeometry.h"
#include "../CLog.h"
#include "../CTexture.h"

#include "../CRenderableComponent.h"
#include "../CTransformComponent.h"
#include "../CRenderable.h"
#include "../CMovementComponent.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace AeroSimulatorEngine;

namespace
{
   // xz-plane
   GLfloat vertices[] =
   {
     -0.5f, 0.0f, -0.5f, 0.0f, 1.0f,
      -0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
      0.5f, 0.0f, -0.5f, 1.0f, 1.0f,
      0.5f, 0.0f, 0.5f, 1.0f, 0.0f
   };

   GLuint indices[] =
   {
      0, 1, 2, 3
   };
}

CLand::CLand(const int id, const int type, std::shared_ptr<CShader>& pShader, const char * textureFilePath, const glm::vec2 & numOfTiles)
   : CGameObject(id, type)
   , mNumOfTiles(numOfTiles)
{
   if (addComponent<CRenderableComponent>())
   {
      LOG("* CLand setting up the renderable component");
      const int numVert = sizeof(vertices) / sizeof(vertices[0]);
      const int numInd = sizeof(indices) / sizeof(indices[0]);
      const int elementsPerVertex = 3;
      const int stride = 5; // 3 coords + 2 tex coords
      scaleVertices(vertices, numVert);
      SGeometryData geometryData(vertices, numVert, indices, numInd, elementsPerVertex, stride);

      getRenderable().setGeometry(geometryData);
      getRenderable().createAndLoadTexture(MAIN_TEXTURE, textureFilePath, DDS);
      getRenderable().setShader(pShader);
      // Repeat the texture for land
      getRenderable().setFlag(REPEAT_TEXTURE, true);
   }

   if (addComponent<CTransformComponent>())
   {
      LOG("* CLand setting up the transform component");
   }

   if (addComponent<CMovementComponent>())
   {
      LOG("* CLand setting up the movement component");
   }

   //LOG("* CLand setting up the collision component");
}

CLand::~CLand()
{
}

CRenderable & CLand::getRenderable()
{
   CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(*this);
   assert(pRenderableComp);

   return pRenderableComp->getRenderable();
}

void CLand::scaleVertices(GLfloat * vertices, const int numOfVertices) const
{
   // Change vertex coordinates for x and z to repeat the texture
   for (int count = 0; count < numOfVertices; ++count)
   {
      if ((count % 5) == 3)
      {
         vertices[count] *= mNumOfTiles.x;
      }
      if ((count % 5) == 4)
      {
         vertices[count] *= mNumOfTiles.y;
      }
   }
}
