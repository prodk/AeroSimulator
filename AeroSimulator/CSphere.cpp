#include "CSphere.h"
#include "CCommonMath.h"
#include "CGeometry.h"
#include "CLine.h"
#include "../src/shaders/CShader.h"
#include "CLog.h"
#include "CTexture.h"

using namespace AeroSimulatorEngine;

#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <cassert>

#include "glm/gtc/matrix_transform.hpp"

namespace
{
   GLuint indices[] = { 0, 1 };
}

CSphere::CSphere()
   : mVertices()
   , mIndices()
   , mNormalLine()
   , mLineGeometry(new CGeometry())
   , mGeometryNormals()
   , mDataNormals()
   , mScaledTRMatrix()
{
   mGeometry.reset(new CGeometry());
   mTexture.reset(new CTexture());

   assert(mLineGeometry);
   assert(mGeometry);
   assert(mTexture);
}

CSphere::~CSphere()
{
}

void CSphere::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   CLog::getInstance().log("\n** CCube::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

void CSphere::addCustomObjects(std::shared_ptr<CShader>& pShader)
{
   if (pShader)
   {
      pShader->link();

      const std::size_t numOfNormals = 0.5*mVertices.size();

      mNormalLine.resize(numOfNormals);          // Lines drawing normals
      mGeometryNormals.resize(numOfNormals);     // Geometry for lines depicting normals

      const glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);

      for (std::size_t count = 0; count < numOfNormals; ++count)
      {
         mGeometryNormals[count].reset(new CGeometry());
         if (mGeometryNormals[count])
         {
            mGeometryNormals[count]->setVertexBuffer(&mDataNormals[2*count]);
            mGeometryNormals[count]->setNumOfVertices(6);

            mGeometryNormals[count]->setIndexBuffer(indices);
            mGeometryNormals[count]->setNumOfIndices(2);

            mGeometryNormals[count]->setNumOfElementsPerVertex(3); ///@todo: probably remove this
            mGeometryNormals[count]->setVertexStride(3); // 3 coords

            mNormalLine[count].reset(new CLine());
            if (mNormalLine[count])
            {
               const glm::vec3& v = mVertices[2*count];

               mNormalLine[count]->setGeometry(mGeometryNormals[count]);
               mNormalLine[count]->setColor(color);
               mNormalLine[count]->setScale(glm::vec3(0.25f, 0.25f, 0.25f));
               mNormalLine[count]->setTranslate(v);
               add(mNormalLine[count].get());

               mNormalLine[count]->setShadersAndBuffers(pShader);
            }
         } // end if mGeometryNormals[count]
      } // end for
   }

   buildModelMatrix(glm::mat4x4(1.0f)); // Bind children positions to the root
}

void CSphere::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
{
   CParentGameObject::buildModelMatrix(parentTRMatrix);

   // For the cube we need to calculate the model matrix
   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
   mModelMatrix = mParentTRMatrix * mScaledTRMatrix;
}

void CSphere::updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt)
{
   const float deltaX = 150.0f * dt;
   calculateTRMatrix();
   mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;

   mRotate.x += deltaX;

   if (trMatrix != mParentTRMatrix)
   {
      mParentTRMatrix = trMatrix;
      mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;
   }

   for (auto * pChild : mChildren)
   {
      if (pChild)
      {
         pChild->updateTRMatrix(mParentByLocalTRMatrix, dt); /// Avoid recalculation on every frame

         ///@todo: probably incorrect, think about it
         if ((mScale.x != 1.0) || (mScale.y != 1.0) || (mScale.z != 1.0))
            pChild->setScale(mScale);
      }
   }
}

void CSphere::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   CParentGameObject::updateModelMatrix(rootModelMatrix);

   // Model matrix of the cube
   mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;
}

bool CSphere::loadTexture(const char * fileName)
{
   const bool result = (0 != mTexture->loadDDSTexture(fileName));

   if (result && (mTexture->getWidth() != mTexture->getHeight()))
   {
      glGenerateTextureMipmap(mTexture->getId());
      CLog::getInstance().log("CSphere::loadTexture(): generating mipmaps for non-square texture, height: ",
         mTexture->getHeight());
   }

   return result;
}

void CSphere::createNonTexturedGeometry()
{
   generateNonTexutredSphere();

   if (mGeometry)
   {
      mGeometry->setVertexBuffer(&mVertices[0]);
      const int numOfVertices = 3 * mVertices.size();
      mGeometry->setNumOfVertices(numOfVertices);

      mGeometry->setIndexBuffer(&mIndices[0]);
      const int numOfIndices = mIndices.size();
      mGeometry->setNumOfIndices(numOfIndices);

      mGeometry->setNumOfElementsPerVertex(3);
      mGeometry->setVertexStride(6); // 6 for coords and normals
   }

   setColor(glm::vec4(0.f, 1.0f, 1.0f, 1.0f));
}

void CSphere::generateNonTexutredSphere()
{
   ///@todo: rename the variables and reconsider their type
   const int Band_Power = 4;  // 2^Band_Power = Total Points in a band.
   const int Band_Points = std::powl(2, Band_Power); // 2^Band_Power
   const int Band_Mask = Band_Points - 2;
   const float Sections_In_Band = (Band_Points / 2.f) - 1.f;
   const int Total_Points = Sections_In_Band*Band_Points;
   // remember - for each section in a band, we have a band
   const float Section_Arc = 2 * M_PI / Sections_In_Band;
   const float R = -1.f; // radius

   int i;
   float x_angle;
   float y_angle;

   mVertices.resize(2*Total_Points); // vertex + normal
   mIndices.resize(Total_Points);

   for (i = 0; i < Total_Points; i++)
   {
      // using last bit to alternate,+band number (which band)
      x_angle = (float)(i & 1) + (i >> Band_Power);

      // (i&Band_Mask)>>1 == Local Y value in the band
      // (i>>Band_Power)*((Band_Points/2)-1) == how many bands
      //  have we processed?
      // Remember - we go "right" one value for every 2 points.
      //  i>>bandpower - tells us our band number
      y_angle = (float)((i&Band_Mask) >> 1) + ((i >> Band_Power)*(Sections_In_Band));

      x_angle *= (float)Section_Arc / 2.0f; // remember - 180° x rot not 360
      y_angle *= (float)Section_Arc;// *-1;

      const int id = 2 * i;
      mVertices[id].x = R*sin(x_angle)*sin(y_angle);
      mVertices[id].y = R*cos(x_angle);
      mVertices[id].z = R*sin(x_angle)*cos(y_angle);

      // Save the normal, it is the same as the vertex if R == 1
      ///@todo: normalize if R != 1
      mVertices[id + 1] = mVertices[id];

      glm::vec3 v1 = glm::vec3(0.0f, 0.f, 0.f);
      mDataNormals.push_back(v1);
      mDataNormals.push_back(mVertices[id]);

      mIndices[i] = i;
   }
}
