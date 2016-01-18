#ifndef AERO_SIMULATOR_CGAME_OBJECT_NEW_H
#define AERO_SIMULATOR_CGAME_OBJECT_NEW_H

#include <unordered_map>
#include <memory>
///@todo: move this file to AeroSimulatorEngine when the old CGameObject is removed

///@todo: move to namespace AeroSimulatorEngine when the old CGameObject is removed
namespace Game
{
   class CComponent;

   class CGameObject
   {
   public:
      CGameObject();
      virtual ~CGameObject();

      /// Add a component to the object
      template <typename T> bool addComponent();

   private:
      // Get the component of type T from the specified (by reference) object
      template <typename T>
      friend T* componentCast(CGameObject& object);

      // Returns a pointer to the component using the type of the component
      template<typename T>
      T* getComponent() { return static_cast<T*>(getComponent(T::getId())); }

      // Returns a pointer to the component using its id
      CComponent* getComponent(unsigned int id);

   private:
      std::unordered_map<unsigned int, std::unique_ptr<CComponent> > mComponents;
   };

   template<typename T>
   T * componentCast(CGameObject & object)
   {
      return object.getComponent<T>();
   }

   template<typename T>
   inline bool CGameObject::addComponent()
   {
      bool added = false;

      auto result = mComponents.find(id);
      if (mComponents.end() == result)
      {
         std::unique_ptr<CComponent> pComponent(new T(this));
         if (pComponent)
         {
            std::pair<unsigned int, std::unique_ptr<CComponent> >
            newComponent(T::getId(), pComponent);
            added = mComponents.insert(newComponent).second;
         }
      }

      return added;
   }
} // namespace

#endif // AERO_SIMULATOR_CGAME_OBJECT_NEW_H
