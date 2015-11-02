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

      void setNumVertices(const int numVertices) { mNumVertices = numVertices; }
      void setNumIndices(const int numIndices) { mNumIndices = numIndices; }

      const int getNumVertices() const { return mNumVertices; }
      const int getNumIndices() const { return mNumIndices; }
      void* getVertexBuffer() const { return mVertices; }
      void* getIndexBuffer() const { return mIndices; }
      void setVertexBuffer(void* pVertices) { mVertices = pVertices; }
      void setIndexBuffer(void* pIndices) { mIndices = pIndices; }

      void setName(const char* name) { if (name) std::strcpy(mName, name); }
      void setNumOfVertexPositions(const int numOfVP) { mNumOfVertexPositions = numOfVP; }
      int getNumOfVertexPositions() const { return mNumOfVertexPositions; }
      void setVertexStride(const int stride) { mVertexStride = stride; }
      int getVertexStride() const { return mVertexStride; }

   private:
      int mNumVertices;
      int mNumIndices;
      void* mVertices;
      void* mIndices;

      int mNumOfVertexPositions;
      int mVertexStride;

      static const unsigned int NAME_MAX_LENGTH = 16;
      char mName[NAME_MAX_LENGTH];
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CGEOMETRY_H
