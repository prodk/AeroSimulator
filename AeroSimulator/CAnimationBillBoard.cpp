#include "CAnimationBillBoard.h"
#include "CTexture.h"

using namespace AeroSimulatorEngine;

CAnimationBillBoard::CAnimationBillBoard()
{
}

CAnimationBillBoard::~CAnimationBillBoard()
{
}

bool CAnimationBillBoard::loadTexture(const char * fileName)
{
   bool result = CBillBoard::loadTexture(fileName);
   if (result)
   {
      ///@todo: think why this is needed to work for the animated sprite
      glGenerateTextureMipmap(mTexture->getId());
   }

   return result;
}
