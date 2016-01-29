// CGeometry.cpp - geometry class implementation

#include "CGeometry.h"

using namespace AeroSimulatorEngine;

CGeometry::CGeometry()
   : mNumVertices(0)
   , mNumIndices(0)
   , mVertices(nullptr)
   , mIndices(nullptr)
   , mNumOfElementsPerVertex(1)
   , mVertexStride(0)
{
}

CGeometry::~CGeometry()
{
}

void * CGeometry::getVertexBuffer() const
{
   return mVertices;
}

void * CGeometry::getIndexBuffer() const
{
   return mIndices;
}

void CGeometry::setVertexBuffer(void * pVertices)
{
   mVertices = pVertices;
}

void CGeometry::setIndexBuffer(void * pIndices)
{
   mIndices = pIndices;
}
