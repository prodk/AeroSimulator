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

      virtual void init() = 0; ///@todo: probably remove this as it is not used, make the destructor pure virtual

      CGameObject * getOwner() const { return mOwner; }

   public:
      enum { TRAHSFORM, RENDERABLE, MOVEMENT };

   private:
      CGameObject * mOwner;
   };
} // namespace

#endif // AERO_SIMULATOR_CCOMPONENT_H
