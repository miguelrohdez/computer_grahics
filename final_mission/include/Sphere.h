#ifndef FINAL_PROJECT_SPHERE_H
#define FINAL_PROJECT_SPHERE_H

#include "Texture.h"
#include "Color.h"

#ifndef PI
	#define PI 3.1415926535897
#endif
#ifndef RESOLUTION_SPHERE
	#define RESOLUTION_SPHERE 60
#endif

class Sphere {
	float radio;
	float scale;
	CTexture texture;
	Color color;
public:
	Sphere();
	Sphere(float radio);
	void draw();
	void drawHalf();
	void drawHalf(float radio, CTexture texture);
	void draw(float radio, CTexture texture);
	void draw(float radio);
	void setRadio(float r);
	void setColor(Color c);
	void setTexture(CTexture t);
};

#endif
