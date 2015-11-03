#include "CVector3.h"

using namespace AeroSimulatorEngine;

CVector3::CVector3()
{
}

CVector3::CVector3(float x, float y, float z)
{
   mValue[0] = x;
   mValue[1] = y;
   mValue[2] = z;
}

CVector3::~CVector3()
{
}
