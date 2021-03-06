// CWin32Renderer.h - declaration of the concrete Renderer for the Windows platform

#ifndef AERO_SIMULATOR_CWIN32_RENDERER_H
#define AERO_SIMULATOR_CWIN32_RENDERER_H

#include "CRenderer.h"
#include "CWin32Window.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <memory>
#include <vector>

namespace AeroSimulatorEngine
{
   class CAppEvent;
   ///@todo: move these classes to CGame
   class CCamera;
   //class CCompositeGameObject;
   //class CParentGameObject;
   //class C3DModel;
   //class CAnimationBillBoard;
   //class CLand;
   //class CSkyBox;
   //class CBillBoard;
   //class CParticleSystem;
   class CQuad;
   class CShader;
   //class CMissile;

   struct SFrameBuffer
   {
      GLuint mFramebuffer;
      GLuint mTexColorBuffer;
      GLuint mRenderBuffer;
      GLuint mTexDepthBuffer;
   };

   //enum eRendererEvents { DEBUG_MODE_EVENT, DEPTHBUF_EVENT };

   class CWin32Renderer : public CRenderer
   {
   public:
      CWin32Renderer(ePriority prio);
      virtual ~CWin32Renderer();

      // Override CTask part
      virtual bool start() override;
      virtual void update(CTask* pTask) override;
      virtual void stop() override;

      // CRenderer part
      virtual bool setRenderContext() override;
      virtual void resetRenderContext() override;

      // CEventHandler part
      virtual void handleEvent(CAppEvent *pEvent) override;

      void setupEvents();

      bool isInitialized() const { return mIsInitialized; }

      void init(const CWin32Window& window);

      ///@todo: remove this method when all the game-related stuff is moved to CGame
      //bool windowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

      ///@todo: move all these set methods to CGame
      //void setAirplaneRoot(CParentGameObject* root);
      //void setSphereRoot(CParentGameObject* root) { if (root) mSphereRoot = root; }

      //void setAirplane(std::shared_ptr<C3DModel>& airplane) { mAirplane = airplane; }
      //void setStars(std::shared_ptr<CAnimationBillBoard>& stars) { if(stars) mStar.push_back(stars); }
      ////void setLand(std::shared_ptr<CLand>& land) { mLand = land; }
      //void setSky(std::shared_ptr<CSkyBox>& sky) { mSky = sky; }

      //void setClouds(const std::vector<std::shared_ptr<CBillBoard> >& clouds) { mClouds = clouds; }
      //void setTurbineFire(std::shared_ptr<CParticleSystem>& fire) { mTurbineFire = fire; }
      //void setTurbineSmoke(std::shared_ptr<CParticleSystem>& smoke) { mTurbineSmoke = smoke; }

      //void setRightMissile(std::shared_ptr<CMissile>& missile) { mRightMissile = missile; }

      //void setExplosion(std::shared_ptr<CAnimationBillBoard>& expl) { mExplosion = expl; }

   private:
      // Override CRenderer part
      virtual void init();
      virtual void destroy();
      virtual void draw(CRenderable* pRenderable);
      virtual void swapBuffers();

      void setupFboQuads();

      void renderSceneToFBOs();
      void renderFBOs();
      void drawScene();
      void drawOpaqueRenderables();
      void drawTransparentRenderables();
      bool createRenderContext();
      void updateFPS(CTask* pTask);

      void setupFbo(SFrameBuffer& fbo, const GLint width, const GLint height);

      void generateAttachmentTexture(SFrameBuffer& fbo);

      //void updateAirplane();
      //void updateCamera();
      //void springButtons();
      //void updateRenderables();

      //void updateInput(); ///@todo: move to CWin32Window or a separate handler when events are implemented
      //void handleCollisions(); ///@todo; move to a special task when events are implemented

   private:
      HDC mDC;
      bool mIsInitialized;
      HGLRC mRenderContext;
      HGLRC mOldRenderContext;
      bool mIsFullScreen;

      ///@todo: move all the camera-related stugg to CGame
      //float mCameraAngleX; // up 'w', down 's'
      //float mCameraAngleY; // left 'a', right 'd'
      ///@todo: probably create an array of cameras later
      //std::shared_ptr<CCamera> mCamera;
      std::unique_ptr<CCamera> mCamera;

      bool mIsDebugMode;
      //bool mIsSetCameraMode;
      //bool mCameraAttached;
      HWND mWndHandle;

      float mFrameDt;

      std::unique_ptr<CQuad> mMainFboQuad;   // Main scene
      std::unique_ptr<CQuad> mHelpFboQuad;   // Helper window
      std::shared_ptr<CShader> mFboShader;
      std::shared_ptr<CShader> mDepthBufferShader;
      SFrameBuffer mMainFbo;
      SFrameBuffer mHelpFbo;

      std::size_t mWndWidth;
      std::size_t mWndHeight;
      bool mDepthBufferMode;  // press '8' to see the depth buffer

      ///@todo: reconsider the approach to spring buttons
      //float mAngleZ; // Angle of rotating around the z-axis of the air plane
      //float mAngleX;

      //CParentGameObject* mAirplaneRoot;
      //CParentGameObject* mSphereRoot;
      //bool mKeyPressed;
      //bool mCameraKeyPressed;
      //bool mThirdKeyPressed;
      //WPARAM mKeyCode;
      //WPARAM mCameraKeyCode;
      //WPARAM mThirdKeyCode;

      ///@todo: when the event framwork is setup, 
      ///@todo: transfer moving objects and collisions to a separaete task
      //glm::mat4x4 mAirplaneMatrix;

      ///@todo: move to the movement/collisions task when events are implemented
      //std::shared_ptr<C3DModel> mAirplane;
      //std::vector<std::shared_ptr<CAnimationBillBoard> > mStar;
      //std::shared_ptr<CLand> mLand; ///@todo: remove
      //std::shared_ptr<CSkyBox> mSky;
      //std::vector<std::shared_ptr<CBillBoard> > mClouds;

      //std::shared_ptr<CParticleSystem> mTurbineFire;
      //std::shared_ptr<CParticleSystem> mTurbineSmoke;
      //std::shared_ptr<CMissile> mRightMissile;
      //std::shared_ptr<CAnimationBillBoard> mExplosion;
   };
} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CWIN32_RENDERER_H
