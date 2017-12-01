//
// Created by luis-elementary on 11/1/17.
//

#ifndef FINAL_PROJECT_PRISMA_H
#define FINAL_PROJECT_PRISMA_H

#include "Texture.h"
#include "GL/glut.h"
#include "Color.h"
#include <stdio.h>

class Prisma {
private:
    CTexture textures[6];
    float x, y, z;
    float scale;
    Color color;
    float repTexture;
	bool flagRepetition;
public:
    Prisma();
    Prisma(float width, float height, float depth, Color color, CTexture textures[6]);
    Prisma(float width, float height, float depth, Color color);
	Prisma(float width, float height, float depth);
    void draw();
    void draw(float offsetY);
    void drawSky(float anim);
    void draw(float w, float h, float d);
    void draw(float w, float h, float d, Color c);
    void draw(float w, float h, float d, CTexture t);
    void draw(float w, float h, float d, CTexture *t);
    void draw(CTexture *t);
    void draw(CTexture t);
    void draw(float *size, CTexture *t);
    void draw(float *size, CTexture t);
    void draw(Color color);
    void setSize(float width, float height, float depth);
    void setSize(float size[3]);
    void setTexture(CTexture t[6]);
    void setTexture(CTexture t);
    void setColor(Color color);
    void setRepetitionTexture(float rt);
    void setScale(float s);
    void toDebug();
	void disableRepetition();
	void enableRepetition();
};


#endif //FINAL_PROJECT_PRISMA_H
