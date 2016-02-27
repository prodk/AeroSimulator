#include "CSkyDom.h"
#include "../AeroSimulator/CRenderableComponent.h"
#include "../AeroSimulator/CTransformComponent.h"
#include "../AeroSimulator/CMovementComponent.h"
#include "../AeroSimulator/CRenderable.h"
#include "../AeroSimulator/CEventManager.h"

#include <vector>
#include <math.h>

using namespace AeroSimulatorEngine;

CSkyDom::CSkyDom(const int id,
                 const int type,
                 std::shared_ptr<CShader>& pShader,
                 const std::string& textureFilePath,
                 const SSphereParams& generateParams)
   : CGameObject(id, type)
{
   addRenderableComponent(pShader, textureFilePath, generateParams);

   // Transform component
   std::vector<int> transformEvents;
   (void)CGameObject::addTransformComponent(transformEvents, "* CSkyDom: ");

   // Movement component
   std::vector<int> moveEvents;
   moveEvents.push_back(eGeneralEvents::UPDATE);
   (void)CGameObject::addMovementComponent(moveEvents, "* CSkyDom: ");
}

CSkyDom::~CSkyDom()
{
}

void CSkyDom::addRenderableComponent(std::shared_ptr<CShader>& pShader, const std::string& textureFilePath, const SSphereParams& generateParams)
{
   std::vector<int> renderableEvents;
   renderableEvents.push_back(eGeneralEvents::UPDATE_RENDERABLE);

   std::vector<GLfloat> vertices;
   std::vector<GLuint> indices;

   CUtils::generateTexturedSphere(vertices, indices, generateParams);

   SGeometryData geometryData(&vertices[0], vertices.size(), &indices[0], indices.size());

   if (CGameObject::addRenderableComponent(pShader, textureFilePath, geometryData, renderableEvents, "* CSkyDom: "))
   {
      // Draw lines in debug mode
      getRenderable().setFlag(eShaderFlags::DRAW_LINES, true);
      getRenderable().set1DParam(eShader1DParams::LINE_WIDTH, 3.0f);
   }
}
