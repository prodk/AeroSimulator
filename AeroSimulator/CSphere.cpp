#include "CSphere.h"
#include "CCommonMath.h"
#include "CGeometry.h"

using namespace AeroSimulatorEngine;

#include <math.h>
#include <stdio.h>
#include <conio.h>

CSphere::CSphere()
   : mVertices()
   , mIndices()
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

void CSphere::generateSphere()
{
   const int Band_Power = 5;  // 2^Band_Power = Total Points in a band.
   const int Band_Points = std::pow(2, 5); // 16 = 2^Band_Power
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

      /*printf("{%f,%f, %f },\n",
         R*sin(x_angle)*sin(y_angle),
         R*cos(x_angle),
         R*sin(x_angle)*cos(y_angle));*/
      mVertices[i].x = R*sin(x_angle)*sin(y_angle);
      mVertices[i].y = R*cos(x_angle);
      mVertices[i].z = R*sin(x_angle)*cos(y_angle);

      mIndices[i] = i;
   }
}

