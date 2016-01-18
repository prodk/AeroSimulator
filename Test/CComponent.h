#ifndef AERO_SIMULATOR_CCOMPONENT_H
#define AERO_SIMULATOR_CCOMPONENT_H

///@todo: move this file to AeroSimulatorEngine when the old CGameObject is removed

///@todo: move to namespace AeroSimulatorEngine when the old CGameObject is removed
namespace Game
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
