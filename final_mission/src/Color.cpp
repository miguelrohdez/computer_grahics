#include "Color.h"
#include <stdio.h>
Color::Color()
{
	printf("Default color\n");
	Color(0,0,0);
}

Color::Color(int r, int g, int b)
{
	Color((float) r, (float) g, (float) b);
}

Color::Color(float r, float g, float b)
{
	this->rgb[0] = r;
	this->rgb[1] = g;
	this->rgb[2] = b;
	printf("COLOR: %f %f %f\n", this->rgb[0], this->rgb[1], this->rgb[2]);
}

Color::Color(int rgb[3])
{
	Color(rgb[0], rgb[1], rgb[2]);
}

Color::Color(float rgb[3])
{
	Color(rgb[0], rgb[1], rgb[2]);
}

float Color::getR() {
	return this->rgb[0];
}

float Color::getG() {
	return this->rgb[1];
}
float Color::getB() {
	return this->rgb[2];
}

float* Color::getRGB() {
	return this->rgb;
}
