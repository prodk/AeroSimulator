#include "CSphere.h"
#include "CCommonMath.h"
#include "CGeometry.h"
#include "CLine.h"
#include "CShader.h"

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
   , mNormals()
   , mLineGeometry(new CGeometry())
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

      mGeometry->setNumOfElementsPerVertex(3); ///@todo: probably remove this
      mGeometry->setVertexStride(3); // 3 coords
   }

   setColor(glm::vec4(0.f, 1.0f, 1.0f, 1.0f));
   setDrawWithLines(true);
}

CSphere::~CSphere()
{
}
void CSphere::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
{
   //CLog::getInstance().log("\n** CCube::setupShadersAndBuffers() **");
   CGameObject::setShadersAndBuffers(pShader);
}

void CSphere::addCustomObjects(std::shared_ptr<CShader>& pShader)
{
   if (mLineGeometry && pShader)
   {
      pShader->link(); // Just in case

      // Set the raw data for the geometry
      mLineGeometry->setVertexBuffer(lineData);
      const int numOfVertices = sizeof(lineData) / sizeof(lineData[0]);
      mLineGeometry->setNumOfVertices(numOfVertices);

      mLineGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mLineGeometry->setNumOfIndices(numOfIndices);

      mLineGeometry->setNumOfElementsPerVertex(3);
      mLineGeometry->setVertexStride(3);

      // Create a normal for each vertex
      ///@todo: put them in one array later
      const std::size_t numOfNormals = 5;// mVertices.size();
      mNormals.resize(numOfNormals);

      const glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);

      for (std::size_t count = 0; count < numOfNormals; ++count)
      {
         mNormals[count].reset(new CLine());
         if (mNormals[count])
         {
            const glm::vec3& v = mVertices[count];
            /*const float length = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
            const glm::vec3 n = glm::vec3(v.x / length, v.y / length, v.z / length);

            const float projectXY = std::sqrtf(n.x*n.x + n.y*n.y);

            float angleY = 90.f;
            if (projectXY > std::numeric_limits<float>::epsilon())
               angleY = std::atan(n.z / projectXY);

            float angleZ = 90.f;
            if (n.x > std::numeric_limits<float>::epsilon())
               angleZ = std::atan(n.y / n.x);

            ///@todo: this is not good, because inside the game object we will again transfrom to radians
            angleY = CCommonMath::radToDeg(angleY);
            angleZ = CCommonMath::radToDeg(angleZ);*/
            // Setup line for the normal
            mNormals[count]->setGeometry(mLineGeometry);
            mNormals[count]->setColor(color);
            //mNormals[count]->setRotate(glm::vec3(0.0f, angleY, angleZ));
            mNormals[count]->setTranslate(-v);
            //mNormals[count]->calculateModelMatrix();
            add(mNormals[count].get());

            mNormals[count]->setShadersAndBuffers(pShader);
         }
      }
   }// end if mLineGeometry

   buildModelMatrix(glm::mat4x4(1.0f));
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
   CParentGameObject::updateTRMatrix(trMatrix);

   // Don't forget to change the cached scaled TR matrix
   if (trMatrix != mParentTRMatrix)
   {
      mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
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
   const int Band_Power = 4;  // 2^Band_Power = Total Points in a band.
   const int Band_Points = std::powl(2, Band_Power); // 2^Band_Power
   const int Band_Mask = Band_Points - 2;
   const float Sections_In_Band = (Band_Points / 2.f) - 1.f;
   const int Total_Points = Sections_In_Band*Band_Points;
   // remember - for each section in a band, we have a band
   const float Section_Arc = 2 * M_PI / Sections_In_Band;
   const float R = -1.f; // radius of 10

   int i;
   float x_angle;
   float y_angle;

   mVertices.resize(Total_Points);
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

      mVertices[i].x = R*sin(x_angle)*sin(y_angle);
      mVertices[i].y = R*cos(x_angle);
      mVertices[i].z = R*sin(x_angle)*cos(y_angle);

      mIndices[i] = i;
   }
}

