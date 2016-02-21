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
      void* getVertexBuffer() const;
      void* getIndexBuffer() const;
      void setVertexBuffer(void* pVertices);
      void setIndexBuffer(void* pIndices);

      ///@todo: remove these 4 methods, stride and elements per vertex are in the shader
      void setNumOfElementsPerVertex(const int numOfEpv) { mNumOfElementsPerVertex = numOfEpv; }
      int getNumOfElementsPerVertex() const { return mNumOfElementsPerVertex; }
      void setVertexStride(const int stride) { mVertexStride = stride; }
      int getVertexStride() const { return mVertexStride; }

   private:
      int mNumVertices;
      int mNumIndices;
      void* mVertices;
      void* mIndices;

      int mNumOfElementsPerVertex; ///@todo: remove
      int mVertexStride; ///@todo: remove
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CGEOMETRY_H
