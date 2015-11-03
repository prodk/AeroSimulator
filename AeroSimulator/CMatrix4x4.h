
#ifndef AERO_SIMULATOR_CMATRIX4X4_H
#define AERO_SIMULATOR_CMATRIX4X4_H

namespace AeroSimulatorEngine
{
   class CVector3;

   class CMatrix4x4
   {
   public:
      CMatrix4x4();
      CMatrix4x4(const CMatrix4x4 & m);
      ~CMatrix4x4();

      float& operator [](const int id)
      {
            return mValue[id];
      }

      const float& operator [](const int id) const
      {
         return mValue[id];
      }

   private:
      float mValue[16];
   };

   // Non-member methods
   CMatrix4x4 translate(const CMatrix4x4& m, const CVector3& v);
   CMatrix4x4 rotateAroundY(const CMatrix4x4& m, const float angle);

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CMATRIX4X4_H

