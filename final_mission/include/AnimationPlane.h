#ifndef FINAL_PROJECT_ANIMATION_AEROPLANE_H
#define FINAL_PROJECT_ANIMATION_AEROPLANE_H

#include "KeyFrame.h"
#include "Prisma.h"
#include "Cylinder.h"
#include "Texture.h"
#define iMaxSteps 400.0f
#define FILE_KEYFRAME_PLANE "../res/Keyframes/keyframe_plane.frm"


class AnimationPlane {
	CTexture textureBody, textureWing;
	KeyFrame frames[20];
	int iCurrentSteps;
	int current;
	int numFrames;
	bool activate;
	float translate[3];
	float rotate[3];
public:
	AnimationPlane();
	AnimationPlane(CTexture textureBody, CTexture textureWing);
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
	void setTextures(CTexture tBody, CTexture tWing);
	void setActivate(bool v);
	void resetValues();
};

#endif
