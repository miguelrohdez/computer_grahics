#ifndef FINAL_PROJECT_ANIMATION_H
#define FINAL_PROJECT_ANIMATION_H

#include "KeyFrame.h"
#include "Prisma.h"
#include "Cylinder.h"
#include "Texture.h"
#define iMaxSteps 90.0f
#define FILE_KEYFRAME_PLANE "keyframe_plane.frm"


class AnimationPlane {
	CTexture texture;
	KeyFrame frames[20];
	int iCurrentSteps;
	int current;
	int numFrames;
	bool activate;
	float translate[3];
	float rotate[3];
public:
	AnimationPlane();
	AnimationPlane(CTexture texture);
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
	void setTexture(CTexture t);
	void setActivate(bool v);
	void resetValues();
};

#endif
