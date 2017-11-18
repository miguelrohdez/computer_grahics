#ifndef FINAL_PROJECT_ANIMATION_H
#define FINAL_PROJECT_ANIMATION_H

#include "KeyFrame.h"
#include "Prisma.h"
using namespace std;

class Animation {
	const int iMaxSteps = 90;
	Prisma prisma;
	vector<KeyFrame> frames(20);
	int iCurrentSteps;
	int playIndex;
	bool activate;

};

#endif
