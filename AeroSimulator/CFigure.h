#ifndef AERO_SIMULATOR_CFIGURE_H
#define AERO_SIMULATOR_CFIGURE_H

#include "CGameObject.h"
#include "CRenderable.h"

namespace AeroSimulatorEngine
{
   struct SRenderableData;
   struct SGeometryData;

   // Allows creating simple geometrical objects: cubes, spheres, lines, etc.
   class CFigure : public CGameObject
   {
   public:
      ///@todo: probably use type as figureType
      CFigure(const int id, const int type, const int figureType, SRenderableData& data);
      virtual ~CFigure();

      enum eFigure {CUBE, SPHERE};

   private:
      SGeometryData getGeometryData(const SRenderableData& data) const;

   private:
      int mFigureType;
   };
} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CFIGURE_H
