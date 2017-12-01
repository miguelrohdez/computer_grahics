#ifndef FINAL_PROJECT_ANIMATION_ROCKET_H
#define FINAL_PROJECT_ANIMATION_ROCKET_H

#include "KeyFrame.h"
#include "Prisma.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Texture.h"
#define iMaxStepsRocket 400.0f
#define FILE_KEYFRAME_ROCKET "../res/Keyframes/keyframe_rocket.frm"


class AnimationRocket {
	CTexture textureBody, textureBase, textureCab, tExplotion;
	KeyFrame frames[20];
	int iCurrentSteps;
	int current;
	int numFrames;
	bool activate;
	float translate[3];
	float rotate[3];
public:
	AnimationRocket();
	AnimationRocket(CTexture textureBody, CTexture textureBase, CTexture textureCab, CTexture tExplotion);
	void loadData();
	void update();
	void draw();
	void saveKeyframe();
	void interpolate();
	void left();
	void right();
	void fordward();
	void backward();
	void up();
	void down();
	void rotateXPositive();
	void rotateXNegative();
	void saveToFile();
	void setTextures(CTexture tBody, CTexture tBase, CTexture tCab, CTexture tExp);
	void setActivate(bool v);
	void resetValues();
};

#endif
