#include "Prisma.h"


Prisma::Prisma() {
    setSize(1,1,1);
    setTexture(NULL);
    setColor(0,0,0);
}

Prisma::Prisma(float width, float height, float depth, CTexture *textures){
    setSize(width, height, depth);
    setTexture(textures);
    setColor(0,0,0);
}

Prisma::Prisma(CTexture texture) {
    setSize(1, 1, 1);
    setTexture(texture);
    setColor(0,0,0);
}

Prisma::Prisma(CTexture *textures) {
    setSize(1,1,1);
    setTexture(textures);
    setColor(0,0,0);
}

Prisma::Prisma(float *size, CTexture texture) {
    setSize(size);
    setTexture(texture);
    setColor(0,0,0);
}

Prisma::Prisma(float *size, CTexture *texures) {
    setSize(size);
    setTexture(textures);
    setColor(0,0,0);
}

Prisma::Prisma(float width, float height, float depth) {
    setSize(width, height, depth);
    setTexture(NULL);
    setColor(0,0,0);
}

Prisma::Prisma(float width, float height, float depth, float *color) {
    setSize(width, height, depth);
    setColor(color);
}

Prisma::Prisma(float width, float height, float depth, CTexture texture) {
    setSize(width, height, depth);
    setTexture(texture);
    setColor(0,0,0);
}

void Prisma::setSize(float *size) {
    for (int i = 0; i < 3; ++i) {
        this->size[i] = 0.5 * size[i];
    }
}

void Prisma::setSize(float width, float height, float depth) {
    this->size[0] = 0.5f * width;
    this->size[1] = 0.5f * height;
    this->size[2] = 0.5f * depth;
}

void Prisma::setTexture(CTexture *t) {
    if (t != NULL) {
        for (int i = 0; i < 6; ++i) {
            this->textures[i] = t[i];
        }
    }
}

Prisma::Prisma(float *size) {
    setSize(size);
    setTexture(NULL);
    setColor(0,0,0);
}

void Prisma::setTexture(CTexture t) {
    if (t.GLindex == NULL) {
        for (int i = 0; i < 6; ++i) {
            this->textures[i] = t;
        }
    }
}

void Prisma::setColor(float *color) {
    for (int i = 0; i < 3; ++i) {
        this->color[i] = color[i];
    }
}

void Prisma::setColor(float r, float g, float b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void Prisma::draw() {
    float v[8][3] = {
            {-size[0], size[1], size[2]},
            {size[0], size[1], size[2]},
            {-size[0], -size[1], size[2]},
            {size[0], -size[1], size[2]},
            {-size[0], -size[1], -size[2]},
            {size[0], -size[1], -size[2]},
            {size[0], size[1], -size[2]},
            {-size[0], size[1], -size[2]}
    };

    //Plano superior
    glColor3f(this->color[0], this->color[1], this->color[2]);
    glBegin(GL_QUADS);
    glVertex3fv(v[0]);
    glVertex3fv(v[1]);
    glVertex3fv(v[6]);
    glVertex3fv(v[7]);
    glEnd();

    //Plano inferior
    glBegin(GL_QUADS);
    glVertex3fv(v[2]);
    glVertex3fv(v[4]);
    glVertex3fv(v[5]);
    glVertex3fv(v[3]);
    glEnd();

    //Plano 1
    glBegin(GL_QUADS);
    glVertex3fv(v[0]);
    glVertex3fv(v[2]);
    glVertex3fv(v[3]);
    glVertex3fv(v[1]);
    glEnd();

    //Plano 2
    glBegin(GL_QUADS);
    glVertex3fv(v[1]);
    glVertex3fv(v[3]);
    glVertex3fv(v[5]);
    glVertex3fv(v[6]);
    glEnd();

    //Plano 3
    glBegin(GL_QUADS);
    glVertex3fv(v[6]);
    glVertex3fv(v[5]);
    glVertex3fv(v[4]);
    glVertex3fv(v[7]);
    glEnd();

    //Plano 4
    glBegin(GL_QUADS);
    glVertex3fv(v[7]);
    glVertex3fv(v[4]);
    glVertex3fv(v[2]);
    glVertex3fv(v[0]);
    glEnd();
}

void Prisma::toDebug() {
    printf("%f %f %f\n", this->size[0], this->size[1], this->size[2]);
}

void Prisma::draw(CTexture t) {
    setTexture(t);
    draw();
}

void Prisma::draw(CTexture *t) {
    setTexture(t);
    draw();
}

void Prisma::draw(float *color) {
    setColor(color);
    draw();
}

void Prisma::draw(float *size, CTexture t) {
    setSize(size);
    setTexture(t);
    draw();
}

void Prisma::draw(float *size, CTexture *t) {
    setSize(size);
    setTexture(t);
    draw();
}

void Prisma::draw(float w, float h, float d) {
    setSize(w,h,d);
    draw();
}

void Prisma::draw(float w, float h, float d, CTexture t) {
    setSize(w,h,d);
    setTexture(t);
    draw();
}

void Prisma::draw(float w, float h, float d, CTexture *t) {
    setSize(w,h,d);
    setTexture(t);
    draw();
}