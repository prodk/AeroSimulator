
#ifndef AERO_SIMULATOR_CVECTOR3_H
#define AERO_SIMULATOR_CVECTOR3_H

namespace AeroSimulatorEngine
{
   class CVector3
   {
   public:
      CVector3();
      CVector3(float x, float y, float z);
      ~CVector3();

      float& operator [] (const int id)
      {
         return mValue[id];
      }

      const float& operator [] (const int id) const
      {
         return mValue[id];
      }

   private:
      float mValue[3];
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CVECTOR3_H
