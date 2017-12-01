#ifndef FINAL_PROJECT_ANIMATION_BALL_H
#define FINAL_PROJECT_ANIMATION_BALL_H

#include "Prisma.h"
#include "Texture.h"

class AnimationBall {
	bool active;
	int contador;
	int state;
	float time;
	float position[3];
public:
	AnimationBall();
	void update();
	void draw(float scale, CTexture texture, int x = 1, int z = 1);
	void activate();
};

#endif
