#include "Plane.h"
#include "Vertex.h"
#include <stdio.h>

Plane::Plane(): color() {
	for (size_t i = 0; i < 4; i++) {
		v[i] = Vertex();
	}
}

Plane::Plane(Vertex v1, Vertex v2, Vertex v3, Vertex v4, CTexture t): color(){
	setVertex(v1, v2, v3, v4);
	setTexture(t);
	calculateRepetitions(1,1);
}

void Plane::draw() {
	glBindTexture(GL_TEXTURE_2D, texture.GLindex);
    glColor3fv(this->color.getRGB());
    glBegin(GL_QUADS);
		glTexCoord2f(0.0, axisARep); glVertex3fv(v[0].getValues());
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[1].getValues());
		glTexCoord2f(axisBRep, 0.0f); glVertex3fv(v[2].getValues());
		glTexCoord2f(axisBRep, axisARep); glVertex3fv(v[3].getValues());
    glEnd();
}

void Plane::draw(float offsetY) {
	glBindTexture(GL_TEXTURE_2D, texture.GLindex);
    glColor3fv(this->color.getRGB());
    glBegin(GL_QUADS);
		glTexCoord2f(0.0, axisARep + offsetY); glVertex3fv(v[0].getValues());
		glTexCoord2f(0.0f, 0.0f + offsetY); glVertex3fv(v[1].getValues());
		glTexCoord2f(axisBRep, 0.0f + offsetY); glVertex3fv(v[2].getValues());
		glTexCoord2f(axisBRep, axisARep + offsetY); glVertex3fv(v[3].getValues());
    glEnd();
}

/*
 * Dibujar con el vector en sentido contrario
 */
void Plane::drawInverse() {
	glBindTexture(GL_TEXTURE_2D, texture.GLindex);
    glColor3fv(this->color.getRGB());
    glBegin(GL_QUADS);
		glTexCoord2f(axisBRep, axisARep); glVertex3fv(v[3].getValues());
		glTexCoord2f(axisBRep, 0.0f); glVertex3fv(v[2].getValues());
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[1].getValues());
		glTexCoord2f(0.0, axisARep); glVertex3fv(v[0].getValues());
    glEnd();
}

/*
 * Para obtener una textura homogénea y que no se
 * vea estirada
 */
void Plane::calculateRepetitions(float repAxisA, float repAxisB) {
	this->axisARep =  (v[0].distance(v[1]) / 20.0) * repAxisA;
	this->axisBRep =  (v[1].distance(v[2]) / 20.0) * repAxisB;
}

void Plane::noRepetition() {
	this->axisARep = 1.0f;
	this->axisBRep = 1.0f;
}

void Plane::setTexture(CTexture t) {
	this->texture = t;
}

void Plane::setColor(Color c) {
	this->color = c;
}

void Plane::setVertex(Vertex axisA1, Vertex axisA2, Vertex axisB1, Vertex axisB2) {
	this->v[0] = axisA1;
	this->v[1] = axisA2;
	this->v[2] = axisB1;
	this->v[3] = axisB2;
}

void Plane::toString() {
	printf("[Plane]\n");
	printf("A: %0.3f | B: %0.3f\n", axisARep, axisBRep);
	color.toString();
	for (size_t i = 0; i < 4; i++) {
		v[i].toString();
	}
	printf("**********\n\n");

}
