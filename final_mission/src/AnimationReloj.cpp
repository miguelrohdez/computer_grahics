#include "AnimationReloj.h"
#include <stdio.h>
#include "Prisma.h"
#include "Color.h"

AnimationReloj::AnimationReloj() {
	this->current = 0;
	this->activate = false;
	this->iCurrentSteps = 0;
	this->rotManecilla = 0.0f;
}

void AnimationReloj::loadData() {
	FILE *data;
	this->numFrames = 0;

	data = fopen(FILE_KEYFRAME_RELOJ, "r");
	if (!data) {
		printf("Error file: %s\n", FILE_KEYFRAME_RELOJ);
		exit(0);
	}
	printf("Cargando frames ...\n");
	while (fread(&frames[this->numFrames], sizeof(KeyFrameReloj), 1, data) == 1) {

		printf("Frame[%d]\n", this->numFrames);
		//frames[this->numFrames].toString(); // Para depurar valores cargados
		this->numFrames++;
	}
	fclose(data);
	resetValues();
	printf("Frames cargados correctamente.\n");
	fflush(stdout);
}

void AnimationReloj::update() {
	if (activate) {
		if (iCurrentSteps >= iMaxStepsReloj) {
			current++;
			if (current > numFrames - 2) {
				activate = false;
				resetValues();
				printf("Termina rotación reloj\n");
			} else {
				iCurrentSteps = 0; //Reset counter
				interpolate();
			}
		} else {
			this->rotManecilla += frames[current].getIncRotManecilla();
			iCurrentSteps++;
		}
	}
}

void AnimationReloj::interpolate() {
	frames[current].setIncRotManecilla((frames[current + 1].getRotManecilla() - frames[current].getRotManecilla()) / iMaxStepsReloj);
}

void AnimationReloj::draw() {

	Prisma p(15.0f, 15.0f, 0.5f, Color(1,1,1));
	p.setTexture(textureBody);
	p.disableRepetition();
	glPushMatrix(); // Pico frontal
		p.draw();
		glRotatef(this->rotManecilla, 0, 0, 1);
		glTranslatef(0,2.5,0.5);
		p.setTexture(textureManecilla);
		p.draw(1.0, 5.0f, 0.2f, Color(0,0,0));
	glPopMatrix();
}

void AnimationReloj::saveKeyframe() {
	frames[current].setRotManecilla(this->rotManecilla);
	frames[current].toString();
	printf("Frame %d guardado.\n", current);
	current++;
}


void AnimationReloj::saveToFile() {
	FILE *data;

	data = fopen(FILE_KEYFRAME_RELOJ, "w");
	if (!data) {
		printf("Error file %s\n", FILE_KEYFRAME_RELOJ);
		exit(0);
	}
	for (int i = 0; i < current; i++) {
		printf("Guardando Frame: \n");
		//frames[i].toString(); // Para depurar valores guardados
		fwrite(&frames[i], sizeof(KeyFrameReloj), 1, data);
	}
	fclose(data);
	printf("Frames guardados exitosamente.\n");
}

void AnimationReloj::setTextures(CTexture tBody, CTexture tManecilla) {
	this->textureBody = tBody;
	this->textureManecilla = tManecilla;
}

void AnimationReloj::setActivate(bool v) {
	this->activate = v;
}

void AnimationReloj::resetValues() {
	this->rotManecilla = frames[0].getRotManecilla();
	current = 0;
}

void AnimationReloj::rotateManecilla() {
	this->rotManecilla -= 5;
}
