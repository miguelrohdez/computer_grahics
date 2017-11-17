#ifndef TEXTURA_H_
#define TEXTURA_H_

#include "Main.h"

class CTexture
{
public:
	char* name;
	unsigned char*  imageData;
	int		bpp;			// Image Color Depth In Bits Per Pixel
	int		width;			// Image Width
	int		height;		    // Image Height
	unsigned int    GLindex;

	bool LoadTGA(const char* filename);		// Loads A TGA File Into Memory
	bool LoadBMP(const char* filename);
	void BuildGLTexture();
	void ReleaseImage();
};

#endif //#ifndef MAIN_H_
