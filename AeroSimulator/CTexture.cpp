#include "CTexture.h"
#include "CLog.h"

#include <stdio.h>
#include <memory>

using namespace AeroSimulatorEngine;

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
   // Actual RGB data
   //unsigned char * data;

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

   ///@todo: probably the buffer should be created/hosted in CApp
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

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



   return 0;
}
