#include "CSphere.h"
#include "CCommonMath.h"
#include "CGeometry.h"
#include "CLine.h"
#include "../src/shaders/CShader.h"
#include "CLog.h"

using namespace AeroSimulatorEngine;

#include <math.h>
#include <stdio.h>
#include <conio.h>

#include "glm/gtc/matrix_transform.hpp"

namespace
{
   ///@todo: probably move to the model as is done for CCube.
   GLfloat lineData[] = {
      0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f
   };

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
   generateSphere();

   mGeometry.reset(new CGeometry());
   if (mGeometry)
   {
      mGeometry->setVertexBuffer(&mVertices[0]);
      const int numOfVertices = 3 * mVertices.size();
      mGeometry->setNumOfVertices(numOfVertices);

      mGeometry->setIndexBuffer(&mIndices[0]);
      const int numOfIndices = mIndices.size();
      mGeometry->setNumOfIndices(numOfIndices);

      mGeometry->setNumOfElementsPerVertex(3);
      mGeometry->setVertexStride(3); // 3 coords, change to 6 when normals are added
   }

   setColor(glm::vec4(0.f, 1.0f, 1.0f, 1.0f));
   //setDrawWithLines(true);
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

      const std::size_t numOfNormals = mVertices.size();

      mNormalLine.resize(numOfNormals);          // Lines drawing normals
      mGeometryNormals.resize(numOfNormals);  // Geometry for lines depicting normals

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
               const glm::vec3& v = mVertices[count];

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

void CSphere::updateTRMatrix(const glm::mat4x4 & trMatrix)
{
   const float deltaX = 1.0f;
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
         pChild->updateTRMatrix(mParentByLocalTRMatrix); /// Avoid recalculation on every frame
      }
   }
}

void CSphere::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
{
   CParentGameObject::updateModelMatrix(rootModelMatrix);

   // Model matrix of the cube
   mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;
}

void CSphere::generateSphere()
{
   ///@todo: rename the variables and reconsider their type
   const int Band_Power = 5;  // 2^Band_Power = Total Points in a band.
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

   mVertices.resize(Total_Points); // vertex + normal
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

      x_angle *= (float)Section_Arc / 2.0f; // remember - 180� x rot not 360
      y_angle *= (float)Section_Arc;// *-1;

      mVertices[i].x = R*sin(x_angle)*sin(y_angle);
      mVertices[i].y = R*cos(x_angle);
      mVertices[i].z = R*sin(x_angle)*cos(y_angle);

      glm::vec3 v1 = glm::vec3(0.0f, 0.f, 0.f);
      mDataNormals.push_back(v1);
      mDataNormals.push_back(mVertices[i]);

      mIndices[i] = i;
   }
}