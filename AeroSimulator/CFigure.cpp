#include "CFigure.h"
#include "CUtils.h"
#include "CTransform.h"
#include "../AeroSimulator/CTransformComponent.h"

using namespace AeroSimulatorEngine;

namespace
{
   GLfloat cubeVertices[] = {
      //front
      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,

      // back
      -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
   };

   GLuint cubeIndices[] = {
      // Stripe 1
      6, 5, 7, 4, // back
      3, 0, // left
      2, 1, // front
      6, 5, // right

      // Stripe 2
      // bottom
      5, 1, 4, 0,

      //// front
      0, 1, 2,
      2, 0, 3,

      //// top
      3, 2, 6,
      6, 3, 7,

      ///@todo: fix wireframe lines
      //// missing line
      4, 6, 5,
   };
}

CFigure::CFigure(const int id, const int type, const int figureType, SRenderableData& data, const CTransform& transform)
   : CGameObject(id, type)
   , mFigureType(figureType)
{
   LOG("CFigure: ", figureType);

   ///@todo: reconsider handling geometry generation for different types of figures
   // Renderable component
   std::vector<int> renderableEvents;
   renderableEvents.push_back(eGeneralEvents::UPDATE_RENDERABLE);

   std::vector<GLfloat> vertices;
   std::vector<GLuint> indices;
   getGeometryData(vertices, indices, data.mSphereParams);

   if ((vertices.size() > 0) && (indices.size() > 0))
   {
      SGeometryData geometryData(&vertices[0], vertices.size(), &indices[0], indices.size());

      if (CGameObject::addRenderableComponent(data.mShader, data.mTextureFilePath, geometryData, renderableEvents, "* CFigure: "))
      {
         // Draw lines in debug mode
         getRenderable().setFlag(eShaderFlags::DRAW_LINES, true);
         getRenderable().set1DParam(eShader1DParams::LINE_WIDTH, 3.0f);
         getRenderable().setVector4Param(eShaderVector4Params::COLOR, data.mColor);
      }
   }

   // Transform component
   std::vector<int> transformEvents;
   (void)CGameObject::addTransformComponent(transformEvents, "* CFigure: ");
   CTransformComponent* pTransformComp = getComponent<CTransformComponent>();
   if (pTransformComp)
   {
      pTransformComp->setTransform(transform);
      CTransform& newTransform = pTransformComp->getTransform();
      newTransform.updateModelMatrix();
   }

   // Movement component
   std::vector<int> moveEvents;
   moveEvents.push_back(eGeneralEvents::UPDATE);
   (void)CGameObject::addMovementComponent(moveEvents, "* CFigure: ");
}

CFigure::~CFigure()
{
}

void CFigure::getGeometryData(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, const SSphereParams * sphereParams) const
{
   switch (mFigureType)
   {
   case eFigure::CUBE:
      for (auto v : cubeVertices)
      {
         vertices.push_back(v);
      }
      for (auto i : cubeIndices)
      {
         indices.push_back(i);
      }
      break;

      ///@todo: handle the situation of no sphere coordinates in a different way
   case eFigure::SPHERE:
      if (sphereParams)
      {
         CUtils::generateTexturedSphere(vertices, indices, *sphereParams);
      }
      else
      {
         LOG("* CFigure() no sphere params were found");
      }
      break;
   }
}
