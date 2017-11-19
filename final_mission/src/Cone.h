#ifndef FINAL_PROJECT_CONE_H
#define FINAL_PROJECT_CONE_H

#include "Texture.h"
#include "Color.h"

class Cone {
	static constexpr float PI = 3.1415926535897;
	static constexpr float resolution = 30;
	float height, radio;
	float scale;
	CTexture texture;
	Color color;
public:
	Cone();
	Cone(float radio, float height, CTexture texture);
	void draw();
	void draw(float radio, float height, CTexture texture);
	void draw(float radio, float height);
	void setRadio(float r);
	void setHeight(float h);
	void setTexture(CTexture t);
	void setColor(Color c);

};

#endif
