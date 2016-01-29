// CGeometry.h - geometry class declaration

#ifndef AERO_SIMULATOR_CGEOMETRY_H
#define AERO_SIMULATOR_CGEOMETRY_H

#include <string>

namespace AeroSimulatorEngine
{
   class CGeometry
   {
   public:
      CGeometry();
      ~CGeometry();

      void setNumOfVertices(const int numVertices) { mNumVertices = numVertices; }
      void setNumOfIndices(const int numIndices) { mNumIndices = numIndices; }

      const int getNumOfVertices() const { return mNumVertices; }
      const int getNumOfIndices() const { return mNumIndices; }
      void* getVertexBuffer() const { return mVertices; }
      void* getIndexBuffer() const { return mIndices; }
      void setVertexBuffer(void* pVertices) { mVertices = pVertices; }
      void setIndexBuffer(void* pIndices) { mIndices = pIndices; }

      void setNumOfElementsPerVertex(const int numOfEpv) { mNumOfElementsPerVertex = numOfEpv; }
      int getNumOfElementsPerVertex() const { return mNumOfElementsPerVertex; }
      void setVertexStride(const int stride) { mVertexStride = stride; }
      int getVertexStride() const { return mVertexStride; }

   private:
      int mNumVertices;
      int mNumIndices;
      void* mVertices;
      void* mIndices;

      int mNumOfElementsPerVertex;
      int mVertexStride;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CGEOMETRY_H
