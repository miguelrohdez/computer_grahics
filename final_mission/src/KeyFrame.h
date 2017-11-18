#ifndef FINAL_PROJECT_KEYFRAME_H
#define FINAL_PROJECT_KEYFRAME_H

class KeyFrame {
	float translation[3];
	float rotation[3];
	float increment[3];
public:
	KeyFrame();
	KeyFrame(float *translations, float *rotations, float *increments);
	KeyFrame(float *translations);
	KeyFrame(float *rotations);
	KeyFrame(float *increments);
	KeyFrame(float *translations, float *rotations);
	KeyFrame(float *rotations, float *increments);
	KeyFrame(float *translations, float *increments);
	void setTranslations(float t[3]);
	void setRotations(float r[3]);
	void setIncrements(float i[3]);
	void setTranslationX(float tX);
	void setTranslationY(float tY);
	void setTranslationZ(float tZ);
	void setRotationX(float rX);
	void setRotationY(float rY);
	void setRotationZ(float rZ);
	void setIncrementX(float iX);
	void setIncrementY(float iY);
	void setIncrementZ(float iZ);
};

class KeyFrameCoche: KeyFrame {

};

#endif
