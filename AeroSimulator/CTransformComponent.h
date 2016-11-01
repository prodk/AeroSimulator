#ifndef AERO_SIMULATOR_CTRANSFORM_COMPONENT_H
#define AERO_SIMULATOR_CTRANSFORM_COMPONENT_H

#include "CComponent.h"
#include "CTransform.h"
#include "CEventHandler.h"
#include <vector>

namespace AeroSimulatorEngine
{
   class CGameObject;

   ///@todo: add processing update event here so that 
   ///@todo: if no Movement component exist the model matrix is still added to the renderable
   class CTransformComponent : public CComponent, public CEventHandler
   {
   public:
      static unsigned int getId() { return mId; }

      explicit CTransformComponent(CGameObject* pOwner);
      virtual ~CTransformComponent();

      CTransform& getTransform() { return mTransform; }
      void setTransform(const CTransform& transform) { mTransform = transform; }

      // CEventHandler part
      void handleEvent(CAppEvent *pEvent) override;

      glm::mat4 getParentMatrix() const { return mParentTrMatrix; }
      void setParentMatrix(const glm::mat4& parent) { mParentTrMatrix = parent; }

      void addChild(CTransformComponent* pChild);

      bool removeChild(CTransformComponent* pChild);

      void updateParentMatrix(const glm::mat4& parent);

   protected:
      CTransform mTransform;
      glm::mat4 mParentTrMatrix; // A parent translate/rotate matrix. It does not contain scale

   private:
      void update();

   private:
      std::vector<CTransformComponent*> mChildren;
      /// Id of the component
      static const unsigned int mId = CComponent::TRANSFORM;
   };
} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CTRANSFORM_COMPONENT_H
