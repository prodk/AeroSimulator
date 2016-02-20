#include "CSkyDom.h"
#include "../AeroSimulator/CRenderableComponent.h"
#include "../AeroSimulator/CTransformComponent.h"
#include "../AeroSimulator/CMovementComponent.h"
#include "../AeroSimulator/CRenderable.h"
#include "../AeroSimulator/CEventManager.h"
#include "../AeroSimulator/include/glm/vec3.hpp"
#include "../AeroSimulator/include/glm/mat4x4.hpp"

#include <vector>
#include <math.h>

using namespace AeroSimulatorEngine;

namespace
{
   ///@todo: place this method to some common file, e.g. common math
   void generateSphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, const SSphereParams& params)
   {
      if ((params.mNumOfCircles > 0) && (params.mNumOfSegments > 0))
      {
         // The generated sphere is z-oriented. We need to rotate it around x by pi/2 to get it y-oriented
         const float angleX = glm::radians(-90.0f);
         const glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
         const glm::mat4 rotateMatrix = glm::rotate(glm::mat4(), angleX, xAxis);

         // Vertices
         const float r = params.mRradius;
         const float deltaTheta = params.mMaxInclination / static_cast<float>(params.mNumOfCircles);
         const float deltaPhi = params.mMaxAzimuth / static_cast<float>(params.mNumOfSegments);

         float theta = 0.0f;
         for (GLuint i = 0; i < params.mNumOfCircles + 1; ++i) ///@note +1
         {
            float phi = 0.0f;
            for (GLuint j = 0; j < params.mNumOfSegments; ++j)
            {
               phi += deltaPhi;
               glm::vec3 pos;
               pos.x = r * std::sin(theta) * std::cos(phi);
               pos.y = r * std::sin(theta) * std::sin(phi);
               pos.z = r * std::cos(theta);

               // Rotate around x to make it y-oriented
               pos = glm::mat3(rotateMatrix) * pos;

               vertices.push_back(pos.x);
               vertices.push_back(pos.y);
               vertices.push_back(pos.z);
            }
            theta += deltaTheta;
         }

         // Indices, the following pattern is generated (for 4 segments):
         // stripe 0: 0415263704; stripe 1: 48596 10 7 11 4 8
         for (GLuint i = 1; i < params.mNumOfCircles + 1; ++i)
         {
            for (GLuint j = 0; j < params.mNumOfSegments; ++j)
            {
               int id = (i - 1) * params.mNumOfSegments + j;
               indices.push_back(id);
               id = i * params.mNumOfSegments + j;
               if (id < (params.mNumOfCircles + 1) * params.mNumOfSegments)
               {
                  indices.push_back(id);
               }

               if (params.mNumOfSegments - 1 == j)
               {
                  indices.push_back((i - 1) * params.mNumOfSegments);
                  indices.push_back(i * params.mNumOfSegments);
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

      ///@todo: put these params to the corresponding shader
      const int elementsPerVertex = 3;
      const int stride = 3;// 5; // 3 coords, ///@todo: add later + 2 tex coords

      std::vector<GLfloat> vertices;
      std::vector<GLuint> indices;
      generateSphere(vertices, indices, generateParams);

      SGeometryData geometryData(&vertices[0], vertices.size(), &indices[0], indices.size(), elementsPerVertex, stride);

      getRenderable().setGeometry(geometryData);
      //getRenderable().createAndLoadTexture(MAIN_TEXTURE, textureFilePath, DDS);
      getRenderable().setShader(pShader);
      //getRenderable().setFlag(eShaderFlags::DRAW_LINES, true);
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
