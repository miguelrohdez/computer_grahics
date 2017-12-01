#include "Sphere.h"

Sphere::Sphere() {
	setRadio(1.0f);
	setColor(Color(1,1,1));
}

Sphere::Sphere(float radio) {
	setRadio(radio);
	setColor(Color(1,1,1));
}

void Sphere::draw(float radio, CTexture texture) {
	setRadio(radio);
	setTexture(texture);
	draw();
}

void Sphere::draw(float radio) {
	setRadio(radio);
	draw();
}

void Sphere::draw() {
	GLdouble angulop = PI / RESOLUTION_SPHERE;
	GLdouble angulom = PI / RESOLUTION_SPHERE;
	GLdouble theta, phi;
	//bool even = true;

	glBindTexture(GL_TEXTURE_2D, texture.GLindex);   // choose the texture to use.

	for(double p =-PI / 2; p < PI / 2; p += angulop)
	{
		for(double m = 0; m < PI * 2; m += angulom)
		{
			//glBegin(GL_LINE_LOOP);
			glBegin(GL_POLYGON);

			//glBegin(GL_POINTS);
			glNormal3f( radio*cos(p + angulop)*cos(m), radio*sin(p + angulop), radio*cos(p + angulop)*sin(m));
			glVertex3f( radio*cos(p + angulop)*cos(m), radio*sin(p + angulop), radio*cos(p + angulop)*sin(m));

			glNormal3f( radio*cos(p + angulop)*cos(m + angulom), radio*sin(p + angulop), radio*cos(p + angulop)*sin(m + angulom));
			glVertex3f( radio*cos(p + angulop)*cos(m + angulom), radio*sin(p + angulop), radio*cos(p + angulop)*sin(m + angulom));

			glNormal3f( radio*cos(p)*cos(m + angulom), radio*sin(p), radio*cos(p)*sin(m + angulom));
			glVertex3f( radio*cos(p)*cos(m + angulom), radio*sin(p), radio*cos(p)*sin(m + angulom));

			glNormal3f(radio*cos(p)*cos(m), radio*sin(p), radio*cos(p)*sin(m));
			glVertex3f( radio*cos(p)*cos(m), radio*sin(p), radio*cos(p)*sin(m));

			glEnd();
		}
	}
}

void Sphere::setTexture(CTexture t) {
	this->texture = t;
}

void Sphere::setColor(Color c) {
	this->color = c;
}

void Sphere::setRadio(float r) {
	this->radio = r;
}
