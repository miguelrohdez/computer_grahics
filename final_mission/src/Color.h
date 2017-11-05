#ifndef FINAL_PROJECT_COLOR_H_
#define FINAL_PROJECT_COLOR_H_

class Color {
private:
	float rgb[3];
public:
	Color();
	Color(int r, int g, int b);
	Color(int *rgb);
	Color(float r, float g, float b);
	Color(float *rgb);
	float getR();
	float getG();
	float getB();
	float* getRGB();
};


#endif
