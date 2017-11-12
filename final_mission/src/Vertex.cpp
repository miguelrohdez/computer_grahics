#include <math.h>
#include "Vertex.h"
#include <stdlib.h>

Vertex::Vertex(float x, float y, float z) {
	setValues(x,y,z);
}

Vertex::Vertex(int x, int y, int z) {
	setValues(x,y,z);
}

Vertex::Vertex(float *c) {
	setValues(c[0],c[1],c[2]);
}

Vertex::Vertex(int *c) {
	setValues(c[0],c[1],c[2]);
}

float* Vertex::getValues() {
	float *values;

	values = (float *) calloc(3, sizeof(float));
	values[0] = this->x;
	values[1] = this->y;
	values[2] = this->z;

	return values;
}

void Vertex::setValues(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vertex::setValues(int x, int y, int z) {
	this->x = (float) x;
	this->y = (float) y;
	this->z = (float) z;
}

float Vertex::distance(Vertex v) {
	return sqrt(pow(this->x - v.getX(), 2)
				+ pow(this->y - v.getY(), 2)
				+ pow(this->z - v.getZ(), 2));
}

float Vertex::getX() {
	return this->x;
}

float Vertex::getY() {
	return this->y;
}

float Vertex::getZ() {
	return this->z;
}
