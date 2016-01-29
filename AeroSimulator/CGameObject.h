#ifndef AERO_SIMULATOR_CGAME_OBJECT_NEW_H
#define AERO_SIMULATOR_CGAME_OBJECT_NEW_H

#include <unordered_map>
#include <memory>

namespace AeroSimulatorEngine
{
   class CComponent;

   ///@todo: any GO can be a parent or a child in any tree.
   ///@todo: foresee such a possibility in this class, including all the necessary transform stuff
   class CGameObject
   {
   public:
      CGameObject();
      virtual ~CGameObject();

      /// Add a component to the object
      template <typename T> bool addComponent();

   protected:
      // Get the component of type T from the specified (by reference) object
      template <typename T>
      friend T* componentCast(CGameObject& object);

      // Returns a pointer to the component using the type of the component
      template<typename T>
      T* getComponent() { return static_cast<T*>(getComponent(T::getId())); }

      // Returns a pointer to the component using its id
      CComponent* getComponent(const unsigned int id) const;

   protected:
      // We have to shared_ptr as unique_ptr refuses to work
      std::unordered_map<unsigned int, std::shared_ptr<CComponent> > mComponents;

      int mId;

      ///@todo: add a list of children here and probably some reference to the parent;
   };

   // Template methods implementation
   template<typename T>
   T * componentCast(CGameObject & object)
   {
      return object.getComponent<T>();
   }

   template<typename T>
   inline bool CGameObject::addComponent()
   {
      bool added = false;

      auto result = mComponents.find(T::getId());
      if (mComponents.end() == result)
      {
         std::shared_ptr<CComponent> pComponent(new T(this));
         if (pComponent)
         {
            std::pair<unsigned int, std::shared_ptr<CComponent> >
            newComponent(T::getId(), pComponent);
            added = mComponents.insert(newComponent).second;
         }
      }

      return added;
   }
} // namespace

#endif // AERO_SIMULATOR_CGAME_OBJECT_NEW_H
