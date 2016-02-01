#include "CCameraManager.h"
#include "CCamera.h"

using namespace AeroSimulatorEngine;

CCameraManager::~CCameraManager()
{
}

int CCameraManager::addCamera(std::shared_ptr<CCamera>& pCamera)
{
   int id = -1;
   if (pCamera)
   {
      id = mCameras.size();
      pCamera->setId(id);
      mCameras.push_back(pCamera);

      // Init the current camera with the first valid value
      if (mCurrentCameraId < 0)
      {
         mCurrentCameraId = id;
      }
   }

   return id;
}

bool CCameraManager::getCamera(const int id, std::shared_ptr<CCamera>& pCamera)
{
   bool result = false;
   if ((id >= 0) && (id < getNumOfCameras()))
   {
      pCamera = mCameras[id];
      result = true;
   }

   return result;
}

void CCameraManager::setCurrentCameraId(const int id)
{
   if ((id >= 0) && (id < getNumOfCameras()))
   {
      mCurrentCameraId = id;
   }
}

CCameraManager::CCameraManager()
   : mCameras()
   , mCurrentCameraId(-1)
{
}
