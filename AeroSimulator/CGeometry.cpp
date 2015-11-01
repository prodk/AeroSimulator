// CGeometry.cpp - geometry class implementation

#include "CGeometry.h"
using namespace AeroSimulatorEngine;

CGeometry::CGeometry()
   : mNumVertices(0)
   , mNumIndices(0)
   , mVertices(nullptr)
   , mIndices(nullptr)
{
}

CGeometry::~CGeometry()
{
}
