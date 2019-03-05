#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

unsigned char* loadTexture(const char* texturePath,int* texWidth, int* texHeight,int* nrChannels)
{
  return stbi_load(texturePath, texWidth,texHeight,nrChannels, 0);
}

void saveTexture(const char* file,int width, int height, unsigned char* texture)
{
  stbi_write_bmp(file,width,height,1,texture);
}

void freeTexture(unsigned char* texture)
{
  stbi_image_free(texture);
}
