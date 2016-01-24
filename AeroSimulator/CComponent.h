#ifndef AERO_SIMULATOR_CCOMPONENT_H
#define AERO_SIMULATOR_CCOMPONENT_H

namespace AeroSimulatorEngine
{
   class CGameObject;

   class CComponent
   {
   public:
      explicit CComponent(CGameObject* pOwner);
      virtual ~CComponent();

      virtual void init() = 0;

      CGameObject * getOwner() const { return mOwner; }

   private:
      CGameObject * mOwner;
   };
} // namespace

#endif // AERO_SIMULATOR_CCOMPONENT_H
