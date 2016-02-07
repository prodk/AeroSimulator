#ifndef AERO_SIMULATOR_CCAMERA_MANAGER_H
#define AERO_SIMULATOR_CCAMERA_MANAGER_H

#include <vector>
#include <memory>

#define GCameraManager CCameraManager::getInstance()

namespace AeroSimulatorEngine
{
   class CCamera;

   // It is a singleton
   class CCameraManager
   {
   public:
      static CCameraManager& getInstance()
      {
         static CCameraManager instance;
         return instance;
      }

      ~CCameraManager();

      // Returns the id of the added camera
      int addCamera(std::shared_ptr<CCamera>& camera);

      ///@todo: foresee a possibility of using several cameras
      // for this we add the argument to this method
      // for a unique camera it is always 0
      bool getCamera(const int id, std::shared_ptr<CCamera>& pCamera);

      void setCurrentCameraId(const int id);
      int getCurrentCameraId() const { return mCurrentCameraId; }
      std::size_t getNumOfCameras() const { return mCameras.size(); }

   private:
      CCameraManager();
      CCameraManager(const CCameraManager&) = delete;
      CCameraManager& operator=(const CCameraManager&) = delete;

      bool isValidId(const int id) const { return (id >= 0) && (static_cast<std::size_t>(id) < getNumOfCameras()); }

   private:
      std::vector<std::shared_ptr<CCamera> > mCameras;
      int mCurrentCameraId;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CCAMERA_MANAGER_H
