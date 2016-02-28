#include "CSkyDom.h"
#include "../AeroSimulator/CRenderableComponent.h"
#include "../AeroSimulator/CTransformComponent.h"
#include "../AeroSimulator/CMovementComponent.h"
#include "../AeroSimulator/CRenderable.h"
#include "../AeroSimulator/CEventManager.h"
#include "../AeroSimulator/CUtils.h"

#include <vector>
#include <math.h>

using namespace AeroSimulatorEngine;

CSkyDom::CSkyDom(const int id,
                 const int type,
                 SRenderableData& data)
   : CGameObject(id, type)
{
   addRenderableComponent(data);

   // Transform component
   std::vector<int> transformEvents;
   ///@todo: think how to avoid using this message as it is not really necessary
   transformEvents.push_back(eGeneralEvents::UPDATE_TRANSFORM);
   (void)CGameObject::addTransformComponent(transformEvents, "* CSkyDom: ");
}

CSkyDom::~CSkyDom()
{
}

void CSkyDom::addRenderableComponent(SRenderableData& data)
{
   if (data.mSphereParams)
   {
      std::vector<int> renderableEvents;
      renderableEvents.push_back(eGeneralEvents::UPDATE_RENDERABLE);

      std::vector<GLfloat> vertices;
      std::vector<GLuint> indices;

      CUtils::generateTexturedSphere(vertices, indices, *data.mSphereParams);

      SGeometryData geometryData(&vertices[0], vertices.size(), &indices[0], indices.size());

      if (CGameObject::addRenderableComponent(data.mShader, data.mTextureFilePath, geometryData, renderableEvents, "* CSkyDom: "))
      {
         // Draw lines in debug mode
         getRenderable().setFlag(eShaderFlags::DRAW_LINES, true);
         getRenderable().set1DParam(eShader1DParams::LINE_WIDTH, 3.0f);
      }
   }
   else
   {
      LOG("* CSkyDom() no sphere params were found");
   }
}
