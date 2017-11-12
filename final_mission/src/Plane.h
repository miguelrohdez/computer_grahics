#ifndef FINAL_PROJECT_PLANE_H
#define FINAL_PROJECT_PLANE_H

#include "Texture.h"
#include "GL/glut.h"
#include "Vertex.h"
#include "Color.h"
#include <stdio.h>

class Plane {
	CTexture texture;
	Color color;
	float axisARep;
	float axisBRep;
	Vertex v[4];
public:
	Plane();
	Plane(CTexture t,
		 Vertex v1=Vertex(0,0,0), Vertex v2=Vertex(1,0,0), Vertex v3=Vertex(1,1,0), Vertex v4=Vertex(0,1,0),
		 int repAxisA=1, int repAxisB=1,
		 Color c=Color(1,1,1));
	Plane(Vertex v1, Vertex v2, Vertex v3, Vertex v4, CTexture t);
	void setTexture(CTexture t);
	void setColor(Color c);
	void setVertex(Vertex v1, Vertex v2, Vertex v3, Vertex v4);
	void draw();
	void drawInverse();
	void toString();
	void calculateRepetitions(int repAxisA, int repAxisB);
};

#endif
