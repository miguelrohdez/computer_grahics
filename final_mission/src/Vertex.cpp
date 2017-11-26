#include <math.h>
#include "Vertex.h"
#include <stdlib.h>
#include <stdio.h>

Vertex::Vertex(){
}

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
	return this->v;
}

void Vertex::setValues(float x, float y, float z) {
	this->v[0] = x;
	this->v[1] = y;
	this->v[2] = z;
}

void Vertex::setValues(int x, int y, int z) {
	this->v[0] = (float) x;
	this->v[1] = (float) y;
	this->v[2] = (float) z;
}

float Vertex::distance(Vertex v) {
	return sqrt(pow(this->v[0] - v.getX(), 2)
				+ pow(this->v[1] - v.getY(), 2)
				+ pow(this->v[2] - v.getZ(), 2));
}

float Vertex::getX() {
	return this->v[0];
}

float Vertex::getY() {
	return this->v[1];
}

float Vertex::getZ() {
	return this->v[2];
}

void Vertex::toString() {
	printf("[VERTEX] X: %0.3f|Y: %0.3f|Z: %0.3f\n", v[0], v[1], v[2]);
}
