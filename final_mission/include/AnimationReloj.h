#ifndef FINAL_PROJECT_ANIMATION_RELOJ_H
#define FINAL_PROJECT_ANIMATION_RELOJ_H

#include "KeyFrame.h"
#include "Prisma.h"
#include "Texture.h"
#define iMaxStepsReloj 1000.0f
#define FILE_KEYFRAME_RELOJ "../res/Keyframes/keyframe_reloj.frm"


class AnimationReloj {
	CTexture textureBody, textureManecilla;
	KeyFrameReloj frames[20];
	int iCurrentSteps;
	int current;
	int numFrames;
	bool activate;
	float rotManecilla;
public:
	AnimationReloj();
	void loadData();
	void update();
	void draw();
	void saveKeyframe();
	void interpolate();
	void rotateManecilla();
	void saveToFile();
	void setTextures(CTexture tBody, CTexture tWing);
	void setActivate(bool v);
	void resetValues();
};

#endif
