#include "CAxesFrame.h"
#include "CLine.h"
#include "CGeometry.h"

#include <cassert>

using namespace AeroSimulatorEngine;

namespace
{
   ///@todo: probably move to the model as is done for CCube.
   GLfloat lineData[] = {
       0.0f, 0.0f, 0.0f,
       1.0f, 0.0f, 0.0f
   };

   GLuint indices[] = { 0, 1 };
}

CAxesFrame::CAxesFrame()
   : mLineGeometry(new CGeometry())
   , mAxisX(new CLine())
   , mAxisY(new CLine())
   , mAxisZ(new CLine())
{
   assert(mLineGeometry);
   assert(mAxisX);
   assert(mAxisY);
   assert(mAxisZ);
}

CAxesFrame::~CAxesFrame()
{
}

void CAxesFrame::buildModel()
{
   // Setup the geometry, use x direction as a default
   if (mLineGeometry)
   {
      mLineGeometry->setVertexBuffer(lineData);
      const int numOfVertices = sizeof(lineData) / sizeof(lineData[0]);
      mLineGeometry->setNumOfVertices(numOfVertices);

      mLineGeometry->setIndexBuffer(indices);
      const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
      mLineGeometry->setNumOfIndices(numOfIndices);

      mLineGeometry->setNumOfElementsPerVertex(3);
      mLineGeometry->setVertexStride(3);
   }

   // Set the geometry
   mAxisX->setGeometry(mLineGeometry);
   mAxisY->setGeometry(mLineGeometry);
   mAxisZ->setGeometry(mLineGeometry);

   // Add the axes
   ///@todo: set color for each axis
   // x
   glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
   mAxisX->setColor(color);
   add(mAxisX.get());

   // y
   color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
   mAxisY->setColor(color);
   mAxisY->setRotate(glm::vec3(0.0f, 0.0f, 90.0f));
   add(mAxisY.get());

   // z
   color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
   mAxisZ->setColor(color);
   mAxisZ->setRotate(glm::vec3(0.0f, 90.0f, 0.0f));
   add(mAxisZ.get());
}
