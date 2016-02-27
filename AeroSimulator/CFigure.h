#ifndef AERO_SIMULATOR_CFIGURE_H
#define AERO_SIMULATOR_CFIGURE_H

#include "CGameObject.h"
#include "CRenderable.h"

namespace AeroSimulatorEngine
{
   struct SRenderableData;
   struct SGeometryData;
   class CTransform;

   // Allows creating simple geometrical objects: cubes, spheres, lines, etc.
   class CFigure : public CGameObject
   {
   public:
      ///@todo: probably use type as figureType
      CFigure(const int id, const int type, const int figureType, SRenderableData& data, const CTransform& transform);
      virtual ~CFigure();

      enum eFigure {CUBE, SPHERE};

   private:
      void getGeometryData(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, const SSphereParams * sphereParams) const;

   private:
      int mFigureType;
   };
} // namespace AeroSimulatorEngine
#endif // AERO_SIMULATOR_CFIGURE_H
