#include "Cylinder.h"

Cylinder::Cylinder() {
	setRadioTop(1.0f);
	setRadioBottom(1.0f);
	setHeight(1.0f);
	setColor(Color(1,1,1));
}

Cylinder::Cylinder(float radio, float height, CTexture texture) {
	setRadioTop(radio);
	setRadioBottom(radio);
	setHeight(height);
	setTexture(texture);
	setColor(Color(1,1,1));
}

Cylinder::Cylinder(float radioTop, float radioBottom, float height, CTexture texture) {
	setRadioTop(radioTop);
	setRadioBottom(radioBottom);
	setHeight(height);
	setTexture(texture);
	setColor(Color(1, 1, 1));
}

void Cylinder::draw(float radioTop, float radioBottom, float height, CTexture texture) {
	setRadioTop(radioTop);
	setRadioBottom(radioBottom);
	setHeight(height);
	setTexture(texture);
	draw();
}

void Cylinder::draw(float radioTop, float radioBottom, float height) {
	setRadioTop(radioTop);
	setRadioBottom(radioBottom);
	setHeight(height);
	draw();
}

void Cylinder::draw(float radio, float height, CTexture texture) {
	setRadioTop(radio);
	setRadioBottom(radio);
	setHeight(height);
	setTexture(texture);
	draw();
}

void Cylinder::draw(float radio, float height) {
	setRadioTop(radio);
	setRadioBottom(radio);
	setHeight(height);
	draw();
}

void Cylinder::draw() {
	float v1[] = {0.0, 0.0, 0.0};
	float v2[] = {0.0, 0.0, 0.0};
	float v3[] = {0.0, 0.0, 0.0};
	float v4[] = {0.0, 0.0, 0.0};
	float v5[] = {0.0, 0.0, 0.0};
	float angle = 2 * PI / resolution;
	//float ctext_s = 1/resolution-1;
	float ctext_s = 1.0/resolution;
	float ctext_t = 0.0;

	for(int i = 0; i < resolution; i++) {
		v2[0] = radioTop*cos(angle*i);
		v2[1] = 0;
		v2[2] = radioTop*sin(angle*i);

		v3[0] = radioTop*cos(angle*(i+1));
		v3[1] = 0;
		v3[2] = radioTop*sin(angle*(i+1));

		v4[0] = radioBottom*cos(angle*i);
		v4[1] = height;
		v4[2] = radioBottom*sin(angle*i);

		v5[0] = radioBottom*cos(angle*(i+1));
		v5[1] = height;
		v5[2] = radioBottom*sin(angle*(i+1));

		glBegin(GL_POLYGON);
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glVertex3f(0.0,0.0,0.0);
			glVertex3fv(v2);
			glVertex3fv(v3);
		glEnd();

		glBegin(GL_POLYGON);
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glVertex3f(0.0,height,0.0);
			glVertex3fv(v4);
			glVertex3fv(v5);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture.GLindex);
		glBegin(GL_POLYGON);
			glNormal3f( v2[0], 0.0f, v2[2]);
			glTexCoord2f(ctext_s*i, 0.0f);		glVertex3fv(v2);
			glTexCoord2f(ctext_s*(i+1), 0.0f);	glVertex3fv(v3);
			glTexCoord2f(ctext_s*(i+1), 1.0f);	glVertex3fv(v5);
			glTexCoord2f(ctext_s*i, 1.0f);		glVertex3fv(v4);
		glEnd();
	}
}

void Cylinder::setTexture(CTexture t) {
	this->texture = t;
}

void Cylinder::setColor(Color c) {
	this->color = c;
}

void Cylinder::setHeight(float h) {
	this->height = h;
}

void Cylinder::setRadioBottom(float r) {
	this->radioBottom = r;
}

void Cylinder::setRadioTop(float r) {
	this->radioTop = r;
}
