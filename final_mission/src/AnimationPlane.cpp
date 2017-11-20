#include "AnimationPlane.h"

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
	int count = 0;

	data = fopen(FILE_KEYFRAME_PLANE, "r");
	if (!data) {
		printf("Error file: %s\n", FILE_KEYFRAME_PLANE);
		exit(0);
	}
	while (!feof(data)) {
		fread(&frames[count], sizeof(KeyFrame), 1, data);
	}
	fclose(data);
}

void AnimationPlane::update() {
	//TODO
}

void AnimationPlane::interpolate() {
	frames[current].setIncrementX((frames[current + 1].getPosX() - frames[current].getPosX()) / iMaxSteps);
	frames[current].setIncrementY((frames[current + 1].getPosY() - frames[current].getPosY()) / iMaxSteps);
	frames[current].setIncrementZ((frames[current + 1].getPosZ() - frames[current].getPosZ()) / iMaxSteps);
}

void AnimationPlane::draw() {
	Cylinder c(10, 50, texture);

	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(rotate[1], 0, 1, 0);
	c.draw();
}

void AnimationPlane::saveKeyframe() {
	frames[current].setTranslationX(this->translate[0]);
	frames[current].setTranslationY(this->translate[1]);
	frames[current].setTranslationX(this->translate[2]);
	frames[current].setRotationY(this->rotate[1]);
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

void AnimationPlane::rotateYNegative() {
	this->rotate[1] += 5;
}

void AnimationPlane::rotateYPositive() {
	this->rotate[1] += 5;
}

void AnimationPlane::saveToFile() {
	FILE *data;

	data = fopen(FILE_KEYFRAME_PLANE, "w");
	if (!data) {
		printf("Error file %s\n", FILE_KEYFRAME_PLANE);
		exit(0);
	}
	for (int i = 0; i < current - 1; i++) {
		fwrite(&frames[i], sizeof(KeyFrame), 1, data);
	}
	fclose(data);
}

void AnimationPlane::setTexture(CTexture t) {
	this->texture = t;
}
