#include "CSkyDom.h"
#include "../AeroSimulator/CRenderableComponent.h"
#include "../AeroSimulator/CTransformComponent.h"
#include "../AeroSimulator/CMovementComponent.h"
#include "../AeroSimulator/CRenderable.h"
#include "../AeroSimulator/CEventManager.h"

#include <vector>
#include <math.h>

using namespace AeroSimulatorEngine;

namespace
{
   //GLfloat vertices[] = {
   //   //front
   //   -0.5f, -0.5f,  0.5f,
   //   0.5f, -0.5f,  0.5f,
   //   0.5f,  0.5f,  0.5f,
   //   -0.5f,  0.5f,  0.5f,

   //   // back
   //   -0.5f, -0.5f, -0.5f,
   //   0.5f, -0.5f, -0.5f,
   //   0.5f,  0.5f, -0.5f,
   //   -0.5f,  0.5f, -0.5f,
   //};

   //GLuint indices[] = {
   //   // Stripe 1
   //   6, 5, 7, 4, // back
   //   3, 0, // left
   //   2, 1, // front
   //   6, 5, // right

   //         // Stripe 2
   //         // bottom
   //   5, 1, 4, 0,

   //   // front
   //   0, 1, 2,
   //   2, 0, 3,

   //   // top
   //   3, 2, 6,
   //   6, 3, 7,

   //   // missing lines
   //   4, 5, 6, 7,
   //   6, 2, 1
   //};
}

namespace
{
   void generateSphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, const SSphereParams& params)
   {
      if ((params.mNumOfCircles > 0) && (params.mNumOfSegments > 0))
      {
         // Vertices
         const float r = params.mRradius;
         const float deltaTheta = params.mMaxInclination / static_cast<float>(params.mNumOfCircles);
         const float deltaPhi = params.mMaxAzimuth / static_cast<float>(params.mNumOfSegments);
         for (float theta = 0.f; theta <= params.mMaxInclination; theta += deltaTheta)
         {
            for (float phi = 0.f; phi < params.mMaxAzimuth; phi += deltaPhi)
            {
               // x
               const GLfloat x = r * std::sin(theta) * std::cos(phi);
               vertices.push_back(x);
               // y
               const GLfloat y = r * std::sin(theta) * std::sin(phi);
               vertices.push_back(y);
               // z
               const GLfloat z = r * std::cos(theta);
               vertices.push_back(z);
            }
         }

         // Indices
         // The following pattern is generated (for 4 segments):
         // stripe 0: 0415263704,
         // stripe 1: 48596 10 7 11 4 8
         for (GLuint i = 1; i < params.mNumOfCircles + 1; ++i)
         {
            for (GLuint j = 0; j < params.mNumOfSegments; ++j)
            {
               indices.push_back((i - 1) * params.mNumOfSegments + j);
               indices.push_back(i * params.mNumOfSegments + j);

               if ((params.mNumOfSegments - 1) == j)
               {
                  indices.push_back((i - 1) * params.mNumOfSegments); // + j == 0
                  indices.push_back(i * params.mNumOfSegments); // + j == 0
               }
            }
         }
      }
   }
}

SSphereParams::SSphereParams(float radius, std::size_t numOfCircles, std::size_t numOfSegments, float maxInclination, float maxAzimuth)
   : mRradius(radius)
   , mNumOfCircles(numOfCircles)
   , mNumOfSegments(numOfSegments)
   , mMaxInclination(maxInclination)
   , mMaxAzimuth(maxAzimuth)
{
}

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

      const int elementsPerVertex = 3;
      const int stride = 3;// 5; // 3 coords, ///@todo: add later + 2 tex coords

      std::vector<GLfloat> vertices;
      std::vector<GLuint> indices;
      generateSphere(vertices, indices, generateParams);

      SGeometryData geometryData(&vertices[0], vertices.size(), &indices[0], indices.size(), elementsPerVertex, stride);

      getRenderable().setGeometry(geometryData);
      //getRenderable().createAndLoadTexture(MAIN_TEXTURE, textureFilePath, DDS);
      getRenderable().setShader(pShader);
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

      CTransformComponent* pTransformComp = getComponent<CTransformComponent>();
      if (pTransformComp)
      {
         CTransform& transform = pTransformComp->getTransform();
         /*const glm::vec3 scale(2.0f, 1.0f, 2.0f); ///@todo: make the scale an arg to the constructor
         transform.setScale(scale);*/
         transform.updateModelMatrix();
      }
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
