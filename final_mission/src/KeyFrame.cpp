#include "KeyFrame.h"
#include <stdio.h>

KeyFrame::KeyFrame() {
	for (int i = 0; i < 3; i++) {
		this->translation[i] = 0.0f;
		this->rotation[i] = 0.0f;
		this->incrementPos[i] = 0.0f;
		this->incrementRot[i] = 0.0f;
	}
}

void KeyFrame::setTranslations(float *t) {
	setTranslationX(t[0]);
	setTranslationY(t[1]);
	setTranslationZ(t[2]);
}

void KeyFrame::setRotations(float *r) {
	setRotationX(r[0]);
	setRotationY(r[1]);
	setRotationZ(r[2]);
}

void KeyFrame::setIncPosX(float i) {
	this->incrementPos[0] = i;
}

void KeyFrame::setIncPosY(float i) {
	this->incrementPos[1] = i;
}

void KeyFrame::setIncPosZ(float i) {
	this->incrementPos[2] = i;
}

void KeyFrame::setIncRotX(float i) {
	this->incrementRot[0] = i;
}

void KeyFrame::setIncRotY(float i) {
	this->incrementRot[1] = i;
}

void KeyFrame::setIncRotZ(float i) {
	this->incrementRot[2] = i;
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

float KeyFrame::getIncPosX() {
	return this->incrementPos[0];
}

float KeyFrame::getIncPosY() {
	return this->incrementPos[1];
}

float KeyFrame::getIncPosZ() {
	return this->incrementPos[2];
}

float KeyFrame::getIncRotX() {
	return this->incrementRot[0];
}

float KeyFrame::getIncRotY() {
	return this->incrementRot[1];
}

float KeyFrame::getIncRotZ() {
	return this->incrementRot[2];
}

void KeyFrame::toString() {
	printf("##### FRAME\n");
	for (int i = 0; i < 3; i++) {
		printf("EJE [%d] -> T: %0.2f, R: %0.2f, IT: %0.2f, IR: %0.2f\n", i, translation[i],
												rotation[i],
												incrementPos[i],
												incrementRot[i]);
	}
	printf("\n");
}
