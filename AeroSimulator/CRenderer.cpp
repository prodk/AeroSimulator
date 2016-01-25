
#include "CRenderer.h"
#include "CRenderable.h"
#include "CLog.h"
#include "CCompositeGameObject.h"

#include <algorithm>

using namespace AeroSimulatorEngine;

CRenderer::CRenderer(ePriority prio)
   : CTask(prio)
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::addRenderable(CRenderable * pRenderable)
{
   if (pRenderable)
   {
      if (pRenderable->isTransparent())
         mTransparentRenderables.push_back(pRenderable);
      else
         mRenderables.push_back(pRenderable);
   }
}

void CRenderer::removeRenderable(CRenderable * pRenderable)
{
   for (auto iter = std::begin(mRenderables); iter != std::end(mRenderables); ++iter)
   {
      if (pRenderable == *iter)
      {
         mRenderables.erase(iter);
         break;
      }
   }
}

bool CRenderer::loadOpenGLExtensions()
{
   bool result = true;

   CLog::getInstance().log("\n* Loading OpenGL extensions");
   std::string strExtension = (const char*)glGetString(GL_EXTENSIONS);
   std::replace(strExtension.begin(), strExtension.end(), ' ', ';');

   CLog::getInstance().log("  OpenGL Extensions:");
   //CLog::getInstance().log(strExtension.c_str());

   // Get the GPU information and the OpenGL extensions
   CLog::getInstance().log("  Video-system information:");
   CLog::getInstance().log("  Videocard: ", (const char*)glGetString(GL_RENDERER));
   CLog::getInstance().log("  Vendor: ", (const char*)glGetString(GL_VENDOR));
   CLog::getInstance().log("  OpenGL Version: ", (const char*)glGetString(GL_VERSION));
   CLog::getInstance().log("");

   return result;
}

void CRenderer::addGameObjectAndItsChildren(CCompositeGameObject * pObject)
{
   if (pObject)
   {
      ///@todo: if add route object if it is parent
      //addRenderable(pObject);

      std::vector<CCompositeGameObject*> tree;
      pObject->traverse(tree);

      /*for (auto * pNode : tree)
      {
         if (pNode)
         {
            addRenderable(pNode);
         }
      }*/
   }
}
