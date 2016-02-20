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
#include <algorithm>

#include "glm/gtc/matrix_transform.hpp"

namespace
{
   GLuint indices[] = { 0, 1 };
}

CSphere::CSphere()
   : mVertices()
   //, mIndices()
   //, mNormalLine()
   , mGeometryNormals()
   , mDataNormals()
   //, mTangentLine()
   , mGeometryTangents()
   , mDataTangents()
   //, mScaledTRMatrix()
{
   //mGeometry.reset(new CGeometry());
   //mTexture.reset(new CTexture());
   //mNormalMapTexture.reset(new CTexture());
   //mAnimationTexture.reset(new CTexture());

  /* assert(mGeometry);
   assert(mTexture);
   assert(mNormalMapTexture);
   assert(mAnimationTexture);*/
}

CSphere::~CSphere()
{
}

//void CSphere::setShadersAndBuffers(std::shared_ptr<CShader>& pShader)
//{
//   CLog::getInstance().log("\n** CCube::setupShadersAndBuffers() **");
//   CGameObject::setShadersAndBuffers(pShader);
//}
//
//void CSphere::addCustomObjects(std::shared_ptr<CShader>& pShader)
//{
//   ///@todo: debug
//   // Dump texture coordinates to a file
//   /*FILE* file = fopen("dumpuv.txt", "w");
//   if (file)
//   {
//      //const std::size_t numOfV = 0.5*mVertices.size();
//      const std::size_t numOfV = mVertices.size()/3;
//      for (std::size_t count = 0; count < numOfV; ++count)
//      {
//         //const glm::vec3& vert = mVertices[2 * count];
//         const glm::vec3& vert = mVertices[3 * count];
//         const float u = std::acos(vert.y / 1.0f) / M_PI;
//         const float v = (std::atan(vert.z/vert.x) + M_PI) / (2.0f *M_PI);
//            fprintf(file, "%d    %lf %lf %lf    %lf %lf\n", count, vert.x, vert.y, vert.z, u, v);
//      }
//      fclose(file);
//   }*/
//   ///@todo: end
//
//   if (pShader)
//   {
//      pShader->link();
//
//      //const std::size_t numOfNormals = 0.5*mVertices.size();
//      const std::size_t numOfNormals = mVertices.size()/3;
//
//      mNormalLine.resize(numOfNormals);          // Lines drawing normals
//      mGeometryNormals.resize(numOfNormals);     // Geometry for lines depicting normals
//
//      const glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
//
//      for (std::size_t count = 0; count < numOfNormals; ++count)
//      {
//         mGeometryNormals[count].reset(new CGeometry());
//         if (mGeometryNormals[count])
//         {
//            mGeometryNormals[count]->setVertexBuffer(&mDataNormals[2*count]);
//            mGeometryNormals[count]->setNumOfVertices(6);
//
//            mGeometryNormals[count]->setIndexBuffer(indices);
//            mGeometryNormals[count]->setNumOfIndices(2);
//
//            mGeometryNormals[count]->setNumOfElementsPerVertex(3); ///@todo: probably remove this
//            mGeometryNormals[count]->setVertexStride(3); // 3 coords
//
//            mNormalLine[count].reset(new CLine());
//            if (mNormalLine[count])
//            {
//               //const glm::vec3& v = mVertices[2*count];
//               const glm::vec3& v = mVertices[3 * count];
//
//               mNormalLine[count]->setGeometry(mGeometryNormals[count]);
//               mNormalLine[count]->setColor(color);
//               mNormalLine[count]->setScale(glm::vec3(0.25f, 0.25f, 0.25f));
//               mNormalLine[count]->setTranslate(v);
//               add(mNormalLine[count].get());
//
//               mNormalLine[count]->setShadersAndBuffers(pShader);
//            }
//         } // end if mGeometryNormals[count]
//      } // end for
//   }
//
//   addTangents(pShader);
//
//   buildModelMatrix(glm::mat4x4(1.0f)); // Bind children positions to the root
//}
//
//void CSphere::buildModelMatrix(const glm::mat4x4 & parentTRMatrix)
//{
//   CParentGameObject::buildModelMatrix(parentTRMatrix);
//
//   // For the cube we need to calculate the model matrix
//   mScaledTRMatrix = glm::scale(mTRMatrix, mScale);
//   mModelMatrix = mParentTRMatrix * mScaledTRMatrix;
//}
//
//void CSphere::updateTRMatrix(const glm::mat4x4 & trMatrix, const float dt)
//{
//   ///@todo: switch rotation for debugging
//   const float deltaX = 150.0f * dt;
//   //const float deltaX = 0.0f * dt;
//   calculateTRMatrix();
//   mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;
//
//   mRotate.x += deltaX;
//
//   if (trMatrix != mParentTRMatrix)
//   {
//      mParentTRMatrix = trMatrix;
//      mParentByLocalTRMatrix = mParentTRMatrix * mTRMatrix;
//   }
//
//   for (auto * pChild : mChildren)
//   {
//      if (pChild)
//      {
//         pChild->updateTRMatrix(mParentByLocalTRMatrix, dt); /// Avoid recalculation on every frame
//
//         ///@todo: probably incorrect, think about it
//         if ((mScale.x != 1.0) || (mScale.y != 1.0) || (mScale.z != 1.0))
//            pChild->setScale(mScale);
//      }
//   }
//}
//
//void CSphere::updateModelMatrix(const glm::mat4x4 & rootModelMatrix)
//{
//   CParentGameObject::updateModelMatrix(rootModelMatrix);
//
//   // Model matrix of the cube
//   mModelMatrix = rootModelMatrix * mParentTRMatrix * mScaledTRMatrix;
//}
//
//bool CSphere::loadTexture(const char * fileName)
//{
//   const bool result = (0 != mTexture->loadDDSTexture(fileName));
//
//   if (result && (mTexture->getWidth() != mTexture->getHeight()))
//   {
//      glGenerateTextureMipmap(mTexture->getId());
//      CLog::getInstance().log("CSphere::loadTexture(): generating mipmaps for non-square texture, height: ",
//         mTexture->getHeight());
//   }
//
//   return result;
//}
//
//bool CSphere::loadNormalMapTexture(const char * filePath)
//{
//   const bool result = (0 != mNormalMapTexture->loadDDSTexture(filePath));
//
//   if (result && (mNormalMapTexture->getWidth() != mNormalMapTexture->getHeight()))
//   {
//      glGenerateTextureMipmap(mNormalMapTexture->getId());
//      CLog::getInstance().log("CSphere::loadNormalMapTexture(): generating mipmaps for non-square texture, height: ",
//         mNormalMapTexture->getHeight());
//   }
//
//   return result;
//}
//
//bool CSphere::loadAnimationTexture(const char * filePath)
//{
//   const bool result = (0 != mAnimationTexture->loadDDSTexture(filePath));
//
//   if (result && (mAnimationTexture->getWidth() != mAnimationTexture->getHeight()))
//   {
//      glGenerateTextureMipmap(mAnimationTexture->getId());
//      CLog::getInstance().log("CSphere::loadNormalMapTexture(): generating mipmaps for non-square texture, height: ",
//         mAnimationTexture->getHeight());
//   }
//
//   return result;
//}

void CSphere::createNonTexturedGeometry()
{
   //generateNonTexutredSphere();

   //if (mGeometry)
   //{
   //   mGeometry->setVertexBuffer(&mVertices[0]);
   //   const int numOfVertices = 3 * mVertices.size();
   //   mGeometry->setNumOfVertices(numOfVertices);

   //   mGeometry->setIndexBuffer(&mIndices[0]);
   //   const int numOfIndices = mIndices.size();
   //   mGeometry->setNumOfIndices(numOfIndices);

   //   mGeometry->setNumOfElementsPerVertex(3);
   //   //mGeometry->setVertexStride(6); // 6 for coords and normals, change to 9 when tangent is added
   //   mGeometry->setVertexStride(9);
   //}

   //setColor(glm::vec4(0.f, 1.0f, 1.0f, 1.0f));
}

void CSphere::update(const float deltaTime)
{
   ///@todo: make members
   const float animationSpeed = 5.0f; // frames per second
   const float animationTime = 1.0f / animationSpeed;

   /*static float timeSinceLastFrame;
   if (timeSinceLastFrame >= animationTime)
   {
      timeSinceLastFrame = 0.0f;

      ++mCurrentFrame;
      mCurrentFrame.x = static_cast<int>(mCurrentFrame.x) % static_cast<int>(mNumOfFrames.x);
      mCurrentFrame.y = static_cast<int>(mCurrentFrame.y) % static_cast<int>(mNumOfFrames.y);
   }*/

   //timeSinceLastFrame += deltaTime;
}

void CSphere::generateNonTexutredSphere()
{
   ///@todo: rename the variables and reconsider their type
   const int Band_Power = 5;  // 2^Band_Power = Total Points in a band.
   const int Band_Points = (int)std::powl(2, Band_Power); // 2^Band_Power
   const int Band_Mask = Band_Points - 2;
   const float Sections_In_Band = (Band_Points / 2.f) - 1.f;
   const int Total_Points = 2;// Sections_In_Band*Band_Points;
   // remember - for each section in a band, we have a band
   const float Section_Arc = 2.0f * (float)M_PI / Sections_In_Band;
   const float R = -1.f; // radius

   int i;
   float x_angle;
   float y_angle;

   //mVertices.resize(2*Total_Points); // vertex + normal
   //mVertices.resize(2 * Total_Points); // vertex + normal + tangent
   //mIndices.resize(Total_Points);

   ///@todo: debug
   //FILE* file = fopen("sphere.txt", "w");
   ///@todo: debug end

   //std::size_t currentId = 0;
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

      //const int id = 2 * i;
      glm::vec3 vertex;
      vertex.x = R*sin(x_angle)*sin(y_angle);
      vertex.y = R*cos(x_angle);
      vertex.z = R*sin(x_angle)*cos(y_angle);
      /*mVertices[id].x = R*sin(x_angle)*sin(y_angle);
      mVertices[id].y = R*cos(x_angle);
      mVertices[id].z = R*sin(x_angle)*cos(y_angle);*/

      ///@todo: this commented code avoids duplicates
      //// Avoid duplicates
      //if (mVertices.end() == std::find(mVertices.begin(), mVertices.end(), vertex))
      //{
      //   // Save position
      //   mVertices.push_back(vertex);
      //   // Save the normal
      //   ///@todo: normalize if R != 1
      //   mVertices.push_back(vertex);

      //   ///@todo: add a place holder for tangent, later calculate tangent here
      //   mVertices.push_back(vertex);

      //   ///@todo: debug
      //   //if (file)
      //   //{
      //      //fprintf(file, "%d    %lf %lf %lf    %lf %lf\n", currentId, vertex.x, vertex.y, vertex.z, x_angle, y_angle);
      //   //}
      //   ///@todo: debug end

      //   glm::vec3 v1 = glm::vec3(0.0f, 0.f, 0.f);
      //   mDataNormals.push_back(v1);
      //   mDataNormals.push_back(vertex);

      //   mIndices.push_back(currentId);
      //   ++currentId;
      //}

      ///@todo: debug
      /*if (file)
      {
         glm::vec3 vert = mVertices[id];
         fprintf(file, "%d    %lf %lf %lf    %lf %lf\n", id, vert.x, vert.y, vert.z, x_angle, y_angle);
      }*/
      ///@todo: debug end

      // Save the normal, it is the same as the vertex if R == 1
      ///@todo: normalize if R != 1
      mVertices.push_back(vertex);
      mVertices.push_back(vertex);
      mVertices.push_back(vertex);

      ///@todo: change this to calculation later
      // Add placeholder for tangents
      //mVertices[id + 2] = mVertices[id];

      glm::vec3 v1 = glm::vec3(0.0f, 0.f, 0.f);
      mDataNormals.push_back(v1);
      mDataNormals.push_back(vertex);

      //mIndices[i] = i;
      //mIndices.push_back(i);
   }

   ///@todo: debug
   //fclose(file);
   ///@todo: end
}

void CSphere::generateTangents()
{
   //const std::size_t numOfV = 0.5*mVertices.size();
   const std::size_t numOfV = mVertices.size()/3;
   for (std::size_t count = 0; count < numOfV; ++count)
   {
      //const glm::vec3& v = mVertices[2 * count];
      const glm::vec3& v = mVertices[3 * count];
      const float theta = std::acos(v.z / 1.0f);
      //float phi = v.y * M_PI/2.0f; ///@todo: Important: v.y is in [-1; 1]

      //if (v.x >= 0.0f)
      //{
         float phi = std::atan(v.y / v.x) + (float)M_PI*0.5f;

         // To get tangent, rotate phi by pi/2 and take into account that cos(theta+pi/2)==sin(theta)
         glm::vec3 tangent;
         tangent.x = std::cos(theta) * std::cos(phi);
         tangent.y = std::cos(theta) * std::sin(phi);
         tangent.z = std::sin(theta);
         glm::normalize(tangent);

         mDataTangents.push_back(glm::vec3(0.f, 0.f, 0.f)); // The vertex is the starting point
         //const float tangentLength = 0.5f;
         const glm::vec3 v2 = tangent;
         mDataTangents.push_back(v2);

         ///@todo: add to generate tangent
         /// Save tangent for shaders
         mVertices[3 * count + 2] = tangent;
      //}
   }
}

void CSphere::addTangents(std::shared_ptr<CShader>& pShader)
{
   //if (pShader)
   //{
   //   pShader->link();

   //   generateTangents();

   //   // Add lines for drawing tangents
   //   const std::size_t numOfTangents = 0.5*mDataTangents.size();

   //   mTangentLine.resize(numOfTangents);          // Lines drawing normals
   //   mGeometryTangents.resize(numOfTangents);     // Geometry for lines depicting normals

   //   const glm::vec4 color(1.0f, 1.0f, 0.0f, 1.0f);

   //   for (std::size_t count = 0; count < numOfTangents; ++count)
   //   {
   //      mGeometryTangents[count].reset(new CGeometry());
   //      ///@todo: add to a method add tangent
   //      if (mGeometryTangents[count])
   //      {
   //         mGeometryTangents[count]->setVertexBuffer(&mDataTangents[2*count]);
   //         mGeometryTangents[count]->setNumOfVertices(6);

   //         mGeometryTangents[count]->setIndexBuffer(indices);
   //         mGeometryTangents[count]->setNumOfIndices(2);

   //         mGeometryTangents[count]->setNumOfElementsPerVertex(3); ///@todo: probably remove this
   //         mGeometryTangents[count]->setVertexStride(3); // 3 coords

   //         mTangentLine[count].reset(new CLine());
   //         if (mTangentLine[count])
   //         {
   //            //const glm::vec3& v = mVertices[2 * count];
   //            const glm::vec3& v = mVertices[3 * count];
   //            mTangentLine[count]->setGeometry(mGeometryTangents[count]);
   //            mTangentLine[count]->setColor(color);
   //            mTangentLine[count]->setScale(glm::vec3(0.25f, 0.25f, 0.25f));
   //            mTangentLine[count]->setTranslate(1.25f*v);
   //            add(mTangentLine[count].get());

   //            mTangentLine[count]->setShadersAndBuffers(pShader);
   //         }
   //      } // end if mGeometryTangents[count]
   //   } // end for
   //}
}
