#ifndef FINAL_PROJECT_KEYFRAME_H
#define FINAL_PROJECT_KEYFRAME_H

class KeyFrame {
	float translation[3];
	float rotation[3];
	float incrementPos[3];
	float incrementRot[3];
public:
	KeyFrame();
	/*
	KeyFrame(float *translations, float *rotations, float *increments);
	KeyFrame(float *translations);
	KeyFrame(float *rotations);
	KeyFrame(float *increments);
	KeyFrame(float *translations, float *rotations);
	KeyFrame(float *rotations, float *increments);
	KeyFrame(float *translations, float *increments);
	*/
	void setTranslations(float t[3]);
	void setRotations(float r[3]);
	void setTranslationX(float tX);
	void setTranslationY(float tY);
	void setTranslationZ(float tZ);
	void setRotationX(float rX);
	void setRotationY(float rY);
	void setRotationZ(float rZ);
	void setIncPosX(float iX);
	void setIncPosY(float iY);
	void setIncPosZ(float iZ);
	void setIncRotX(float iX);
	void setIncRotY(float iY);
	void setIncRotZ(float iZ);
	void toString();
	float getPosX();
	float getPosY();
	float getPosZ();
	float getRotX();
	float getRotY();
	float getRotZ();
	float getIncPosX();
	float getIncPosY();
	float getIncPosZ();
	float getIncRotX();
	float getIncRotY();
	float getIncRotZ();
};

class KeyFrameCoche: KeyFrame {

};

#endif
