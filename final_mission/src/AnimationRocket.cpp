#include "AnimationRocket.h"
#include <stdio.h>
#include "Prisma.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Color.h"

/* Inicializar Valores */
AnimationRocket::AnimationRocket() {
	this->current = 0;
	this->activate = false;
	this->iCurrentSteps = 0;
	for (int i = 0; i < 3; i++) {
		translate[i] = 0.0f;
		rotate[i] = 0.0f;
	}
}

void AnimationRocket::loadData() {
	FILE *data;
	this->numFrames = 0;

	data = fopen(FILE_KEYFRAME_ROCKET, "r");
	if (!data) {
		printf("Error file: %s\n", FILE_KEYFRAME_ROCKET);
		exit(0);
	}
	//printf("Cargando frames ...\n");
	while (fread(&frames[this->numFrames], sizeof(KeyFrame), 1, data) == 1) {
		//printf("Frame[%d]\n", this->numFrames);
		//frames[this->numFrames].toString(); // Para depurar valores cargados
		this->numFrames++;
	}
	fclose(data);
	resetValues();
	//printf("Frames cargados correctamente.\n");
	//fflush(stdout);
}

void AnimationRocket::update() {
	if (activate) {
		if (iCurrentSteps >= iMaxStepsRocket) {
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

void AnimationRocket::interpolate() {
	frames[current].setIncPosX((frames[current + 1].getPosX() - frames[current].getPosX()) / iMaxStepsRocket);
	frames[current].setIncPosY((frames[current + 1].getPosY() - frames[current].getPosY()) / iMaxStepsRocket);
	frames[current].setIncPosZ((frames[current + 1].getPosZ() - frames[current].getPosZ()) / iMaxStepsRocket);
	frames[current].setIncRotX((frames[current + 1].getRotX() - frames[current].getRotX()) / iMaxStepsRocket);
}

void AnimationRocket::draw() {
	Cylinder c(25, 250, textureBody);
	//Prisma p(10.f, 10.f, 10.f, Color(1,1,1));
	Cone co(1, 1, textureBase);

	//p.setTexture(textureBase);

	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(rotate[0], 0, 1, 0);
	glRotatef(-90, 0, 0, 1);

	// Pico frontal
	glPushMatrix();
		glRotatef(90, 0, 0, 1);
		c.draw();
		glTranslatef(0, 250, 0);
		c.draw(25, 5, 30, textureCab);
	glPopMatrix();

	// Base
	glPushMatrix();
		glTranslatef(13, 0, 0);
		glRotatef(90, 0, 0, 1);
		co.draw(30, 40);
	glPopMatrix();

	// Cohetes laterales
	glPushMatrix();
		glTranslatef(-5, 0, 35);
		glRotatef(90, 0, 0, 1);
		c.draw(10, 150, textureBase);
		glTranslatef(0, 150, 0);
		c.draw(10, 2, 15);

		// Base
		glPushMatrix();
			glTranslatef(0, -153, 0);
			glRotatef(0, 0, 0, 1);
			co.draw(10, 20);
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-5, 0, -35);
		glRotatef(90, 0, 0, 1);
		c.draw(10, 150, textureBase);
		glTranslatef(0, 150, 0);
		c.draw(10, 2, 15);

		// Base
		glPushMatrix();
			glTranslatef(0, -153, 0);
			glRotatef(0, 0, 0, 1);
			co.draw(10, 20);
		glPopMatrix();
	glPopMatrix();


}

void AnimationRocket::saveKeyframe() {
	frames[current].setTranslationX(this->translate[0]);
	frames[current].setTranslationY(this->translate[1]);
	frames[current].setTranslationZ(this->translate[2]);
	frames[current].setRotationX(this->rotate[0]);
	frames[current].toString();
	printf("Frame %d guardado.\n", current);
	current++;
}

void AnimationRocket::left() {
	this->translate[0] += 10;
}

void AnimationRocket::right() {
	this->translate[0] -= 10;
}

void AnimationRocket::fordward() {
	this->translate[2] += 10;
}

void AnimationRocket::backward() {
	this->translate[2] -= 10;
}

void AnimationRocket::up() {
	this->translate[1] += 10;
}

void AnimationRocket::down() {
	this->translate[1] -= 10;
}

void AnimationRocket::rotateXNegative() {
	this->rotate[0] += 5;
}

void AnimationRocket::rotateXPositive() {
	this->rotate[0] -= 5;
}

void AnimationRocket::saveToFile() {
	FILE *data;

	data = fopen(FILE_KEYFRAME_ROCKET, "w");
	if (!data) {
		printf("Error file %s\n", FILE_KEYFRAME_ROCKET);
		exit(0);
	}
	for (int i = 0; i < current; i++) {
		printf("Guardando Frame: \n");
		//frames[i].toString(); // Para depurar valores guardados
		fwrite(&frames[i], sizeof(KeyFrame), 1, data);
	}
	fclose(data);
	printf("Frames guardados exitosamente.\n");
}

void AnimationRocket::setTextures(CTexture tBody, CTexture tBase, CTexture tCab) {
	this->textureBody = tBody;
	this->textureBase = tBase;
	this->textureCab = tCab;
}

void AnimationRocket::setActivate(bool v) {
	this->activate = v;
}

void AnimationRocket::resetValues() {
	this->translate[0] = frames[0].getPosX();
	this->translate[1] = frames[0].getPosY();
	this->translate[2] = frames[0].getPosZ();
	this->rotate[0] = frames[0].getRotX();
	current = 0;
}
