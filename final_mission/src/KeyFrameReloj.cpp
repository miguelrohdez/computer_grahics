#include "KeyFrame.h"
#include "stdio.h"
KeyFrameReloj::KeyFrameReloj(): KeyFrame() {
	this->rotManecilla = 0.0f;
	this->incRotManecilla = 0.0f;
}

float KeyFrameReloj::getRotManecilla() {
	return this->rotManecilla;
}

float KeyFrameReloj::getIncRotManecilla() {
	return this->incRotManecilla;
}

void KeyFrameReloj::setRotManecilla(float r) {
	this->rotManecilla = r;
}

void KeyFrameReloj::setIncRotManecilla(float r) {
	this->incRotManecilla = r;
}

void KeyFrameReloj::toString() {
	printf("****KeyFrameReloj\n");
	printf("rotation: %0.4f\n", this->rotManecilla);
	printf("\n");
}
