
#include "CRenderer.h"

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
