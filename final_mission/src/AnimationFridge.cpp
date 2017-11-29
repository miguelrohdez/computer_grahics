#include "AnimationFridge.h"

AnimationFridge::AnimationFridge() {
    this->activate1 = false;
    this->activate2 = false;
    this->state1 = 0;
    this->state2 = 0;
    this->angleDoor1 = 0.0f;
    this->angleDoor2 = 0.0f;
}

void AnimationFridge::update() {
    if (this->activate1) {
        switch (this->state1) {
            case 0:
                if (this->angleDoor1 <= 90.0) {
                    this->angleDoor1 += VELOCITY;
                } else {
                    this->activate1 = false;
                    this->state1 = 1;
                }
                break;
            case 1:
                if (this->angleDoor1 >= 0.0) {
                    this->angleDoor1 -= VELOCITY;
                } else {
                    this->activate1 = false;
                    this->state1 = 0;
                }
                break;
        }
    }
    if (this->activate2) {
        switch (this->state2) {
            case 0:
                if (this->angleDoor2 <= 90.0) {
                    this->angleDoor2 += VELOCITY;
                } else {
                    this->activate2 = false;
                    this->state2 = 1;
                }
                break;
            case 1:
                if (this->angleDoor2 >= 0.0) {
                    this->angleDoor2 -= VELOCITY;
                } else {
                    this->activate2 = false;
                    this->state2 = 0;
                }
                break;
        }
    }
}

void AnimationFridge::draw(float scale, CTexture *r, CTexture *d, CTexture *dn) {
    Prisma p(1, 1, 1);
	p.disableRepetition();
	float h2 = 5.0f * scale;
	float h3 = 10.0f * scale;
	float h1 = 15.0 * scale;
	float a1 = 5.5f * scale;
	float a2 = 0.5f * scale;
	float l = 6.0f * scale;

	glPushMatrix();
		glRotatef(this->angleDoor1, 0, 1, 0);
		glTranslatef(0, 0, l/2);
		p.draw(a2, h3, l, d);
	glPopMatrix();
	glPushMatrix();
		glRotatef(this->angleDoor2, 0, 1, 0);
		glTranslatef(0, (h3 + h2) / 2, l/2);
		p.draw(a2, h2, l, dn);
	glPopMatrix();
	glTranslatef(0, (h3 + h2) / 2, l/2);
	glTranslatef(-(a2 + a1) / 2, -h3 / 2, 0);
	p.draw(a1, h1, l, r);
}

void AnimationFridge::activateDoor1() {
    this->activate1 = true;
}

void AnimationFridge::activateDoor2() {
    this->activate2 = true;
}
