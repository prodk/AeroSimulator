#include "CFigure.h"
#include "CUtils.h"

using namespace AeroSimulatorEngine;

CFigure::CFigure(const int id, const int type, const int figureType, SRenderableData& data)
   : CGameObject(id, type)
   , mFigureType(figureType)
{
   if (data.mSphereParams)
   {
      std::vector<int> renderableEvents;
      renderableEvents.push_back(eGeneralEvents::UPDATE_RENDERABLE);

      if (CGameObject::addRenderableComponent(data.mShader, data.mTextureFilePath, getGeometryData(), renderableEvents, "* CSkyDom: "))
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

CFigure::~CFigure()
{
}

SGeometryData CFigure::getGeometryData(const SRenderableData& data) const
{
   std::vector<GLfloat> vertices;
   std::vector<GLuint> indices;

   switch (mFigureType)
   {
   case eFigure::CUBE:
      break;

   default:
   case eFigure::SPHERE:
      CUtils::generateTexturedSphere(vertices, indices, *data.mSphereParams);
      break;
   }

   return SGeometryData(&vertices[0], vertices.size(), &indices[0], indices.size());
}
