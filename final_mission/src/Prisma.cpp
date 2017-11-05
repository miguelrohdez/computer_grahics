#include "Prisma.h"

Prisma::Prisma(float width, float height, float depth, Color color, CTexture *textures):
color{color}, x{0.5*width}, y{0.5*height}, z{0.5*depth}
{
    setTexture(textures);
}

void Prisma::setSize(float *size) {
    this->x = size[0];
    this->y = size[1];
    this->z = size[2];
}


void Prisma::setSize(float width, float height, float depth) {
    this->x = 0.5f * width;
    this->y = 0.5f * height;
    this->z = 0.5f * depth;
}

void Prisma::setTexture(CTexture *t) {
    if (t != NULL) {
        for (int i = 0; i < 6; ++i) {
            this->textures[i] = t[i];
        }
    }
}

void Prisma::setTexture(CTexture t) {
    if (t.GLindex != NULL) {
        for (int i = 0; i < 6; ++i) {
            this->textures[i] = t;
        }
    }
}

void Prisma::setColor(Color color) {
    this->color = color;
}

void Prisma::draw() {
    float v[8][3] = {
            {-x,  y,  z},
            { x,  y,  z},
            {-x, -y,  z},
            { x, -y,  z},
            {-x, -y, -z},
            { x, -y, -z},
            { x,  y, -z},
            {-x,  y, -z}
    };

    //Plano superior
    glColor3fv(this->color.getRGB());
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
    printf("Size %f %f %f\n", this->x, this->y, this->z);
    printf("RGB: %f %f %f\n", this->color.getR(), this->color.getG(), this->color.getB());
}

void Prisma::draw(CTexture t) {
    setTexture(t);
    draw();
}

void Prisma::draw(CTexture *t) {
    setTexture(t);
    draw();
}

void Prisma::draw(Color color) {
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
