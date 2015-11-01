// CGeometry.h - geometry class declaration

#ifndef AERO_SIMULATOR_CGEOMETRY_H
#define AERO_SIMULATOR_CGEOMETRY_H

namespace AeroSimulatorEngine
{
   class CGeometry
   {
   public:
      CGeometry();
      ~CGeometry();

      void SetNumVertices(const int numVertices) { mNumVertices = numVertices; }
      void SetNumIndices(const int numIndices) { mNumIndices = numIndices; }

      const int GetNumVertices() const { return mNumVertices; }
      const int GetNumIndices() const { return mNumIndices; }
      void* GetVertexBuffer() const { return mVertices; }
      void* GetIndexBuffer() const { return mIndices; }
      void SetVertexBuffer(void* pVertices) { mVertices = pVertices; }
      void SetIndexBuffer(void* pIndices) { mIndices = pIndices; }

   private:
      //static const unsigned int NAME_MAX_LENGTH = 16;
      //char mName[NAME_MAX_LENGTH];
      int mNumVertices;
      int mNumIndices;
      void* mVertices;
      void* mIndices;
   };

} // namespace AeroSimulatorEngine

#endif // AERO_SIMULATOR_CGEOMETRY_H
