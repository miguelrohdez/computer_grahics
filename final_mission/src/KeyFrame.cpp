#include "KeyFrame.h"

KeyFrame::KeyFrame() {
	for (int i = 0; i < 3; i++) {
		this->translation[i] = 0.0f;
		this->rotation[i] = 0.0f;
		this->increment[i] = 0.0f;
	}
}

void KeyFrame::setTranslations(float *translations) {
	setTranslationX(translations[0]);
	setTranslationY(translations[1]);
	setTranslationZ(translations[2]);
}

void KeyFrame::setRotations(float *rotations) {
	setRotationX(rotations[0]);
	setRotationY(rotations[1]);
	setRotationZ(rotations[2]);
}

void KeyFrame::setIncrements(float *rotations) {
	setIncrementX(increment[0]);
	setIncrementY(increment[1]);
	setIncrementZ(increment[2]);
}

void KeyFrame::setIncrementX(float i) {
	this->increment[0] = i;
}

void KeyFrame::setIncrementY(float i) {
	this->increment[1] = i;
}

void KeyFrame::setIncrementZ(float i) {
	this->increment[2] = i;
}

void KeyFrame::setRotationX(float r) {
	this->rotation[0] = r;
}

void KeyFrame::setRotationY(float r) {
	this->rotation[1] = r;
}

void KeyFrame::setRotationZ(float r) {
	this->rotation[2] = r;
}

void KeyFrame::setTranslationX(float t) {
	this->translation[0] = t;
}

void KeyFrame::setTranslationY(float t) {
	this->translation[1] = t;
}

void KeyFrame::setTranslationZ(float t) {
	this->translation[2] = t;
}

void KeyFrame::toString() {

}

float KeyFrame::getPosX() {
	return this->translation[0];
}

float KeyFrame::getPosY() {
	return this->translation[1];
}

float KeyFrame::getPosZ() {
	return this->translation[2];
}

float KeyFrame::getRotX() {
	return this->rotation[0];
}

float KeyFrame::getRotY() {
	return this->rotation[1];
}

float KeyFrame::getRotZ() {
	return this->rotation[2];
}

float KeyFrame::getIncX() {
	return this->rotation[0];
}

float KeyFrame::getIncY() {
	return this->rotation[1];
}

float KeyFrame::getIncZ() {
	return this->rotation[2];
}
