#include "CLand.h"
#include "../AeroSimulator/CGeometry.h"
#include "../AeroSimulator/CLog.h"
#include "../AeroSimulator/CTexture.h"

#include "../AeroSimulator/CRenderableComponent.h"
#include "../AeroSimulator/CTransformComponent.h"
#include "../AeroSimulator/CRenderable.h"
#include "../AeroSimulator/CMovementComponent.h"
#include "../AeroSimulator/CEventManager.h"
#include "../AeroSimulator/CEventHandler.h"
#include "../AeroSimulator/CUtils.h"

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

CLand::CLand(const int id, const int type, SRenderableData& data, const glm::vec2 & numOfTiles, const glm::vec3& size)
   : CGameObject(id, type)
   , mNumOfTiles(numOfTiles)
{
   // Renderable component
   std::vector<int> renderableEvents;
   renderableEvents.push_back(eGeneralEvents::UPDATE_RENDERABLE);

   scaleVertices(vertices, ARRAYLEN(vertices));
   SGeometryData geometryData(vertices, ARRAYLEN(vertices), indices, ARRAYLEN(indices));

   if (CGameObject::addRenderableComponent(data.mShader, data.mTextureFilePath, geometryData, renderableEvents, "* CLand: "))
   {
      getRenderable().setFlag(REPEAT_TEXTURE, true);
   }

   // Transform component
   addTransformComponent(size);

   // Movement component
   /*std::vector<int> moveEvents;
   moveEvents.push_back(eGeneralEvents::UPDATE);
   (void)CGameObject::addMovementComponent(moveEvents, "* CLand: ");*/

   //LOG("* CLand setting up the collision component");
}

CLand::~CLand()
{
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

void CLand::addTransformComponent(const glm::vec3& size)
{
   std::vector<int> transformEvents;
   transformEvents.push_back(eGeneralEvents::UPDATE_TRANSFORM);
   if (CGameObject::addTransformComponent(transformEvents, "* CLand: "))
   {
      CTransformComponent* pTransformComp = getComponent<CTransformComponent>();
      if (pTransformComp)
      {
         CTransform& transform = pTransformComp->getTransform();
         transform.setScale(size);
         transform.updateModelMatrix();
      }
   }
}
