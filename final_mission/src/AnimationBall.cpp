#include "AnimationBall.h"
#include "Sphere.h"
#include <math.h>
#include <stdio.h>

#define R 5
#define K 10

AnimationBall::AnimationBall() {
	this->active = false;
	this->state = 0;
	this->time = 0;
	this->contador = 0;
	for (size_t i = 0; i < 3; i++) {
		this->position[i] = 0.0f;
	}
}

void AnimationBall::update() {
	if (this->active) {
		if (time < 10) {
			position[0] = R * cos(time);
			position[2] = R * sin(time);
			position[1] = K * time;
			time += 0.05;
		} else {
			time = 0;
		}
		contador = 0;
	}
}

void AnimationBall::draw(float scale, CTexture texture, int x, int z) {
	float radio = 1.0f * scale;
    Sphere s(radio);
	glPushMatrix();
		//glRotatef(this->angleDoor1, 0, 1, 0);
		glTranslatef(this->position[0] * x, this->position[1], this->position[2] * z);
		s.draw(radio, texture);
	glPopMatrix();
}

void AnimationBall::activate() {
	this->active = true;
}
