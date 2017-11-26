#ifndef FINAL_PROJECT_VERTEX_H
#define FINAL_PROJECT_VERTEX_H

class Vertex {
	float v[3];
public:
	Vertex();
	Vertex(float x, float y, float z);
	Vertex(int x, int y, int z);
	Vertex(float *c);
	Vertex(int *c);
	float* getValues();
	void setValues(float x, float y, float z);
	void setValues(int x, int y, int z);
	float distance(Vertex x);
	float getX();
	float getY();
	float getZ();
	void toString();
};

#endif
