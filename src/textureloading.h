unsigned char* loadTexture(const char* texturePath,int* texWidth, int* texHeight,int* nrChannels);
void freeTexture(unsigned char* texture);
void saveTexture(const char* file,int width, int height, unsigned char* texture);
