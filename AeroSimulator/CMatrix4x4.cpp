#include "CMatrix4x4.h"
#include "CVector3.h"
#include <memory>

using namespace AeroSimulatorEngine;

CMatrix4x4::CMatrix4x4()
{
   // Init as a unity matrix
   std::memset(mValue, 0, 16 * sizeof(float));
   mValue[0] = 1.0f;
   mValue[5] = 1.0f;
   mValue[10] = 1.0f;
   mValue[15] = 1.0f;
}

CMatrix4x4::CMatrix4x4(const CMatrix4x4& m)
{
   std::memcpy(mValue, m.mValue, 16*sizeof(float));
}

CMatrix4x4::~CMatrix4x4()
{
}

CMatrix4x4 translate(const CMatrix4x4 & m, const CVector3 & v)
{
   CMatrix4x4 result(m);
   result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];

   return result;
}
