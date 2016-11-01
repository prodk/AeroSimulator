#ifndef AERO_SIMULATOR_CGAME_OBJECT_NEW_H
#define AERO_SIMULATOR_CGAME_OBJECT_NEW_H

#include "CEventManager.h"
#include "CLog.h"

#include "../AeroSimulator/include/glm/mat4x4.hpp"

#include <unordered_map>
#include <memory>
#include <string>
#include <map>

namespace AeroSimulatorEngine
{
   class CComponent;
   class CShader;
   struct SGeometryData;
   class CRenderable;

   class CGameObject
   {
   public:
      CGameObject();
      CGameObject(const int id, const int type);
      virtual ~CGameObject();

      /// Add a component to the object
      template <typename T> bool addComponent();

      /// Returns true if the object has a component T
      template <typename T> bool hasComponent();

      void setFrameDt(const float dt) { mFrameDt = dt; }
      float getFrameDt() const { return mFrameDt; }

      void addChild(std::shared_ptr<CGameObject>& pChild);
      bool removeChild(std::shared_ptr<CGameObject>& pChild);

      int type() const { return mType; }

      virtual void move(); // move the object if needed

      virtual bool getChildren(std::map<int, std::shared_ptr<CGameObject>> & kids);

   protected:
      // Get the component of type T from the specified (by reference) object
      template <typename T>
      friend T* componentCast(CGameObject& object);

      // Returns a pointer to the component using the type of the component
      template <typename T>
      T* getComponent() const { return static_cast<T*>(getComponent(T::getId())); }

      // Returns a pointer to the component using its id
      CComponent* getComponent(const unsigned int id) const;

      // Adding standard components
      typedef std::shared_ptr<CShader> tShaderPtr;
      virtual bool addRenderableComponent(tShaderPtr& pShader,
                                          const std::string& textureFilePath,
                                          const SGeometryData& geometryData,
                                          const std::vector<int>& events,
                                          const char * msg);

      virtual bool addTransformComponent(const std::vector<int>& events, const char * msg);

      virtual bool addMovementComponent(const std::vector<int>& events, const char * msg);

      //@todo: implement when needed: virtual bool addCollisionComponent(const std::vector<int>& events, const char * msg);
      //@todo: if several cameras needed,implement addCameraComponent();

      void registerEvents(const std::vector<int>& events, const char * msg) const;

      template <typename T>
      void attachEvents(const std::vector<int>& events, const char * msg) const;

      CRenderable & getRenderable(); ///@todo: think how to make this method const

   protected:
      // We have to shared_ptr as unique_ptr refuses to work
      std::unordered_map<unsigned int, std::shared_ptr<CComponent> > mComponents;
      int mId; // Unique id of the object
      int mType;  // Type of the object, e.g. land, camera etc.
      float mFrameDt; ///@todo: probably remove it from here and move to some singleton
   };

   typedef std::shared_ptr<CGameObject> tGoSharedPtr;

   // Template methods implementation
   template <typename T>
   T * componentCast(CGameObject & object)
   {
      return object.getComponent<T>();
   }

   template <typename T>
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

   template<typename T>
   inline bool CGameObject::hasComponent()
   {
      return (nullptr != this->getComponent<T>());
   }

   template <typename T>
   inline void CGameObject::attachEvents(const std::vector<int>& events, const char * msg) const
   {
      T * pComponent = getComponent<T>();
      if (pComponent)
      {
         for (auto e : events)
         {
            GEventManager.attachEvent(e, *pComponent);
            LOG(msg, " attachEvents(): ", e);
         }
      }
   }
} // namespace

#endif // AERO_SIMULATOR_CGAME_OBJECT_NEW_H
