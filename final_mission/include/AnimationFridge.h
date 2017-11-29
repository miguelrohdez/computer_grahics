#ifndef FINAL_PROJECT_ANIMATION_FRIDGE_H
#define FINAL_PROJECT_ANIMATION_FRIDGE_H

#include "Prisma.h"
#include "Texture.h"

#define VELOCITY 1.0f
class AnimationFridge {
	bool activate1;
	bool activate2;
	int state1;
	int state2;
	float angleDoor1;
	float angleDoor2;
public:
	AnimationFridge();
	void update();
	void draw(float scale, CTexture *r, CTexture *d, CTexture *dn);
	void activateDoor1();
	void activateDoor2();
};

#endif
