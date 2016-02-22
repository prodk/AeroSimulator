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
                 const char * textureFilePath,
                 const SSphereParams& generateParams)
   : CGameObject(id, type)
{
   addRenderableComponent(pShader, textureFilePath, generateParams);
   addTransformComponent();
   addMovementComponent();
}

CSkyDom::~CSkyDom()
{
}

void CSkyDom::addRenderableComponent(std::shared_ptr<CShader>& pShader, const char * textureFilePath, const SSphereParams& generateParams)
{
   if (addComponent<CRenderableComponent>())
   {
      LOG("* CSkyDom setting up the renderable component");

      std::vector<GLfloat> vertices;
      std::vector<GLuint> indices;
      //CUtils::generateSphere(vertices, indices, generateParams); // for color shader

      CUtils::generateTexturedSphere(vertices, indices, generateParams);

      SGeometryData geometryData(&vertices[0], vertices.size(), &indices[0], indices.size());

      getRenderable().setGeometry(geometryData);
      getRenderable().createAndLoadTexture(MAIN_TEXTURE, textureFilePath, DDS);
      getRenderable().setShader(pShader);

      ///@todo: add line rendering in the debug mode
      getRenderable().setFlag(eShaderFlags::DRAW_LINES, true);
      getRenderable().set1DParam(eShader1DParams::LINE_WIDTH, 3.0f);

      if (GEventManager.registerEvent(eGeneralEvents::UPDATE_RENDERABLE))
      {
         LOG("CSkyDom::CRenderableComponent: UPDATE event registered");
      }
      GEventManager.attachEvent(eGeneralEvents::UPDATE_RENDERABLE, *getComponent<CRenderableComponent>());
   }
}

void CSkyDom::addTransformComponent()
{
   if (addComponent<CTransformComponent>())
   {
      LOG("* CSkyDom setting up the transform component");
   }
}

void CSkyDom::addMovementComponent()
{
   if (addComponent<CMovementComponent>())
   {
      LOG("* CSkyDom setting up the movement component");

      if (GEventManager.registerEvent(eGeneralEvents::UPDATE))
      {
         LOG("CSkyDom:CMovementComponent: UPDATE event registered: ");
      }

      GEventManager.attachEvent(eGeneralEvents::UPDATE, *getComponent<CMovementComponent>());
   }
}

CRenderable & CSkyDom::getRenderable()
{
   CRenderableComponent* pRenderableComp = componentCast<CRenderableComponent>(*this);
   assert(pRenderableComp);

   return pRenderableComp->getRenderable();
}
