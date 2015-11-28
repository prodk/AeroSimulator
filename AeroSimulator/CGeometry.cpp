// CGeometry.cpp - geometry class implementation

#include "CGeometry.h"
#include "CLog.h"
using namespace AeroSimulatorEngine;

CGeometry::CGeometry()
   : mNumVertices(0)
   , mNumIndices(0)
   , mVertices(nullptr)
   , mIndices(nullptr)
   , mNumOfElementsPerVertex(1)
   , mVertexStride(0)
{
   //CLog::getInstance().log("CGeometry created");
}

CGeometry::~CGeometry()
{
}
