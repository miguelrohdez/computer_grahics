#include "Cone.h"

Cone::Cone() {
	setHeight(1.0f);
	setRadio(1.0f);
	setColor(Color(1, 1, 1));
}

Cone::Cone(float radio, float height, CTexture texture) {
	setRadio(radio);
	setHeight(height);
	setTexture(texture);
	setColor(Color(1, 1, 1));
}

void Cone::draw(float radio, float height) {
	setRadio(radio);
	setHeight(height);
	draw();
}

void Cone::draw(float radio, float height, CTexture texture) {
	setRadio(radio);
	setHeight(height);
	setTexture(texture);
	draw();
}

void Cone::draw()
{

	float v1[] = {0.0, 0.0, 0.0};
	float v2[] = {0.0, 0.0, 0.0};
	float v3[] = {0.0, 0.0, 0.0};
	float v4[] = {0.0, 0.0, 0.0};

	float angulo = 2 * PI / resolution;
	float ctext_s = 1.0 / resolution;

	glBindTexture(GL_TEXTURE_2D, texture.GLindex);   // choose the texture to use.
	
	for(int i = 0; i < resolution; i++) {
		v2[0] = radio * cos(angulo * i);
		v2[1] = 0;
		v2[2] = radio * sin(angulo * i);

		v3[0] = radio * cos(angulo * (i + 1));
		v3[1] = 0;
		v3[2] = radio * sin(angulo * (i + 1));

		//glBegin(GL_LINE_LOOP);
        glBegin(GL_POLYGON); //Construye Base
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glVertex3f(0.0,0.0,0.0);
			glVertex3fv(v2);
			glVertex3fv(v3);
		glEnd();


		glBindTexture(GL_TEXTURE_2D, texture.GLindex);
		glBegin(GL_TRIANGLES);
			glNormal3f( v2[0], 0.0f, v2[2]);
			glTexCoord2f(ctext_s * i, 0.0f);		glVertex3fv(v2);
			glTexCoord2f(ctext_s * (i + 1), 0.0f);	glVertex3fv(v3);
			glTexCoord2f(ctext_s * i, 1.0f);	glVertex3f(0.0, height, 0.0);
		glEnd();
	}
}

void Cone::setRadio(float r) {
	this->radio = r;
}

void Cone::setHeight(float h) {
	this->height = h;
}

void Cone::setTexture(CTexture t) {
	this->texture = t;
}

void Cone::setColor(Color c) {
	this->color = c;
}
