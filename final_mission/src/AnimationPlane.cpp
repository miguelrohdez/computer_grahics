#include "AnimationPlane.h"
#include <stdio.h>

AnimationPlane::AnimationPlane() {
	this->current = 0;
	this->activate = false;
	this->iCurrentSteps = 0;
	for (int i = 0; i < 3; i++) {
		translate[i] = 0.0f;
		rotate[i] = 0.0f;
	}
}

void AnimationPlane::loadData() {
	FILE *data;
	this->numFrames = 0;

	data = fopen(FILE_KEYFRAME_PLANE, "r");
	if (!data) {
		printf("Error file: %s\n", FILE_KEYFRAME_PLANE);
		exit(0);
	}
	printf("Cargando frames ...\n");
	while (fread(&frames[this->numFrames], sizeof(KeyFrame), 1, data) == 1) {
		printf("Frame[%d]\n", this->numFrames);
		frames[this->numFrames].toString();
		this->numFrames++;
	}
	fclose(data);
	resetValues();
	printf("Frames cargados correctamente.");
	fflush(stdout);
}

void AnimationPlane::update() {
	if (activate) {
		if (iCurrentSteps >= iMaxSteps) {
			current++;
			if (current > numFrames - 2) {
				activate = false;
				resetValues();
				printf("Termina animacion.\n");
			} else {
				iCurrentSteps = 0; //Reset counter
				interpolate();
			}
		} else {
			translate[0] += frames[current].getIncPosX();
			translate[1] += frames[current].getIncPosY();
			translate[2] += frames[current].getIncPosZ();
			rotate[0] += frames[current].getIncRotX();
			iCurrentSteps++;
		}
	}
}

void AnimationPlane::interpolate() {
	frames[current].setIncPosX((frames[current + 1].getPosX() - frames[current].getPosX()) / iMaxSteps);
	frames[current].setIncPosY((frames[current + 1].getPosY() - frames[current].getPosY()) / iMaxSteps);
	frames[current].setIncPosZ((frames[current + 1].getPosZ() - frames[current].getPosZ()) / iMaxSteps);
	frames[current].setIncRotX((frames[current + 1].getRotX() - frames[current].getRotX()) / iMaxSteps);
}

void AnimationPlane::draw() {
	Cylinder c(10, 50, texture);

	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(90, 0, 0, 1);
	glRotatef(rotate[0], 1, 0, 0);
	c.draw();
}

void AnimationPlane::saveKeyframe() {
	frames[current].setTranslationX(this->translate[0]);
	frames[current].setTranslationY(this->translate[1]);
	frames[current].setTranslationZ(this->translate[2]);
	frames[current].setRotationX(this->rotate[0]);
	frames[current].toString();
	printf("Frame %d guardado.\n", current);
	current++;
}

void AnimationPlane::left() {
	this->translate[0] += 10;
}

void AnimationPlane::right() {
	this->translate[0] -= 10;
}

void AnimationPlane::fordward() {
	this->translate[2] += 10;
}

void AnimationPlane::backward() {
	this->translate[2] -= 10;
}

void AnimationPlane::up() {
	this->translate[1] += 10;
}

void AnimationPlane::down() {
	this->translate[1] -= 10;
}

void AnimationPlane::rotateXNegative() {
	this->rotate[0] += 5;
}

void AnimationPlane::rotateXPositive() {
	this->rotate[0] -= 5;
}

void AnimationPlane::saveToFile() {
	FILE *data;

	data = fopen(FILE_KEYFRAME_PLANE, "w");
	if (!data) {
		printf("Error file %s\n", FILE_KEYFRAME_PLANE);
		exit(0);
	}
	for (int i = 0; i < current; i++) {
		printf("Guardando Frame: \n");
		frames[i].toString();
		fwrite(&frames[i], sizeof(KeyFrame), 1, data);
	}
	fclose(data);
	printf("Frames guardados exitosamente.\n");
}

void AnimationPlane::setTexture(CTexture t) {
	this->texture = t;
}

void AnimationPlane::setActivate(bool v) {
	this->activate = v;
}

void AnimationPlane::resetValues() {
	this->translate[0] = frames[0].getPosX();
	this->translate[1] = frames[0].getPosY();
	this->translate[2] = frames[0].getPosZ();
	this->rotate[0] = frames[0].getRotX();
	current = 0;
}
