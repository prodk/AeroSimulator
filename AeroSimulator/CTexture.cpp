#include "CTexture.h"
#include "CLog.h"

#include <stdio.h>
#include <memory>

using namespace AeroSimulatorEngine;

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

CTexture::CTexture()
   : mId(0)
   , mWidth(0)
   , mHeight(0)
{
}

CTexture::~CTexture()
{
}

GLuint CTexture::loadBmpTexture(const char * filePath)
{
   // Data read from the header of the BMP file
   unsigned char header[54]; // Each BMP file begins by a 54-bytes header
   unsigned int dataPos;     // Position in the file where the actual data begins
   unsigned int width, height;
   unsigned int imageSize;   // = width*height*3

   // Open the file
   FILE * file = fopen(filePath, "rb");
   if (!file) { printf("Image could not be opened\n"); return 0; }

   if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
      printf("Not a correct BMP file\n");
      return 0;
   }

   if (header[0] != 'B' || header[1] != 'M') {
      printf("Not a correct BMP file\n");
      return 0;
   }

   // Read ints from the byte array
   dataPos = *(int*)&(header[0x0A]);
   imageSize = *(int*)&(header[0x22]);
   width = *(int*)&(header[0x12]);
   height = *(int*)&(header[0x16]);

   // Some BMP files are misformatted, guess missing information
   if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
   if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

   // Create a buffer
   std::shared_ptr<unsigned char> data;
   data.reset(new unsigned char[imageSize]);

   // Read the actual data from the file into the buffer
   fread(data.get(), 1, imageSize, file);

   //Everything is in memory now, the file can be closed
   fclose(file);

   mWidth = width;
   mHeight = height;
   ///@todo: place to a separate method setup or bind().
   // Create one OpenGL texture
   glGenTextures(1, &mId);

   // "Bind" the newly created texture : all future texture functions will modify this texture
   glBindTexture(GL_TEXTURE_2D, mId);

   // Give the image to OpenGL
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.get());

   // Unbind the texture
   glBindTexture(GL_TEXTURE_2D, 0);

   return 1;
}

GLuint CTexture::loadDDSTexture(const char * filePath)
{
   unsigned char header[124];

   FILE *fp;

   /* try to open the file */
   fp = fopen(filePath, "rb");
   if (fp == NULL)
      return 0;

   /* verify the type of file */
   char filecode[4];
   fread(filecode, 1, 4, fp);
   if (strncmp(filecode, "DDS ", 4) != 0) {
      fclose(fp);
      return 0;
   }

   /* get the surface desc */
   fread(&header, 124, 1, fp);

   unsigned int height = *(unsigned int*)&(header[8]);
   unsigned int width = *(unsigned int*)&(header[12]);
   unsigned int linearSize = *(unsigned int*)&(header[16]);
   unsigned int mipMapCount = *(unsigned int*)&(header[24]);
   unsigned int fourCC = *(unsigned int*)&(header[80]);

   // After the header is the actual data : all the mipmap levels, successively. We can read them all in one batch
   std::shared_ptr<unsigned char> buffer;

   /* how big is it going to be including all mipmaps? */
   const unsigned int bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;

   buffer.reset(new unsigned char[bufsize]);
   fread(buffer.get(), 1, bufsize, fp);
   /* close the file pointer */
   fclose(fp);

   // We need to convert the 'fourCC' flag into a value that OpenGL understands
   unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
   unsigned int format;
   switch (fourCC)
   {
   case FOURCC_DXT1:
      format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      break;
   case FOURCC_DXT3:
      format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      break;
   case FOURCC_DXT5:
      format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      break;
   default:
      buffer.reset();
      return 0;
   }

   // Creating the texture
   mWidth = width;
   mHeight = height;

   glGenTextures(1, &mId);

   // "Bind" the newly created texture : all future texture functions will modify this texture
   glBindTexture(GL_TEXTURE_2D, mId);

   // Fill each mipmap one after another :
   unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
   unsigned int offset = 0;

   /* load the mipmaps */
   for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
   {
      unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
      glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
         0, size, buffer.get() + offset);

      offset += size;
      width /= 2;
      height /= 2;
   }

   return mId;
}
