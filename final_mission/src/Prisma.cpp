#include "Prisma.h"

Prisma::Prisma(float width, float height, float depth, Color color, CTexture *textures):
color{color}, x{0.5*width}, y{0.5*height}, z{0.5*depth}, repTexture{1}
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
    glBindTexture(GL_TEXTURE_2D, textures[0].GLindex);
    //Plano superior
    glColor3fv(this->color.getRGB());
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[0]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[1]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[6]);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[7]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[1].GLindex);
    //Plano inferior
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[2]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[4]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[5]);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[3]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[2].GLindex);
    //Plano 1
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[0]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[2]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[3]);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[1]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[3].GLindex);
    //Plano 2
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[1]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[3]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[5]);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[6]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[4].GLindex);
    //Plano 3
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[6]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[5]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[4]);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[7]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[5].GLindex);
    //Plano 4
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[7]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[4]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[2]);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[0]);
    glEnd();
}


void Prisma::drawSky() {
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
    glBindTexture(GL_TEXTURE_2D, textures[0].GLindex);
    //Plano superior
    glColor3fv(this->color.getRGB());
    glBegin(GL_QUADS);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[7]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[6]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[1]);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[0]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[1].GLindex);
    //Plano inferior
    glBegin(GL_QUADS);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[3]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[5]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[4]);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[2]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[2].GLindex);
    //Plano 1
    glBegin(GL_QUADS);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[1]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[3]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[2]);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[0]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[3].GLindex);
    //Plano 2
    glBegin(GL_QUADS);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[6]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[5]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[3]);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[1]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[4].GLindex);
    //Plano 3
    glBegin(GL_QUADS);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[7]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[4]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[5]);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[6]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[5].GLindex);
    //Plano 4
    glBegin(GL_QUADS);
    glTexCoord2f(repTexture, 0.0f); glVertex3fv(v[0]);
    glTexCoord2f(repTexture, repTexture); glVertex3fv(v[2]);
    glTexCoord2f(0.0f, repTexture); glVertex3fv(v[4]);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[7]);
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

void Prisma::draw(float w, float h, float d, Color c) {
    setSize(w,h,d);
    setColor(c);
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

void Prisma::setRepetitionTexture(int rt) {
    this->repTexture = rt;
}
