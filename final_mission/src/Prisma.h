//
// Created by luis-elementary on 11/1/17.
//

#ifndef FINAL_PROJECT_PRISMA_H
#define FINAL_PROJECT_PRISMA_H

#include "Texture.h"
#include "GL/glut.h"
#include <stdio.h>

class Prisma {
private:
    CTexture textures[6];
    float size[3];
    float color[3];
public:
    Prisma();
    Prisma(float size[3]);
    Prisma(CTexture texure);
    Prisma(CTexture texures[6]);
    Prisma(float size[3],  CTexture texures[6]);
    Prisma(float size[3],  CTexture texure);
    Prisma(float width, float height, float depth);
    Prisma(float width, float height, float depth, float color[3]);
    Prisma(float width, float height, float depth, CTexture texture);
    Prisma(float width, float height, float depth, CTexture textures[6]);
    void draw();
    void draw(float w, float h, float d);
    void draw(float w, float h, float d, CTexture t);
    void draw(float w, float h, float d, CTexture *t);
    void draw(CTexture *t);
    void draw(CTexture t);
    void draw(float *size, CTexture *t);
    void draw(float *size, CTexture t);
    void draw(float *color);
    void setSize(float width, float height, float depth);
    void setSize(float size[3]);
    void setTexture(CTexture t[6]);
    void setTexture(CTexture t);
    void setColor(float colores[3]);
    void setColor(float r, float g, float b);
    void toDebug();
};


#endif //FINAL_PROJECT_PRISMA_H
