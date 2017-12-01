#include "Prisma.h"
#include "Plane.h"

Prisma::Prisma() {
    Color c(1,1,1);
    setSize(1,1,1);
    setColor(c);
    setRepetitionTexture(1);
    setScale(1.0f);
}

Prisma::Prisma(float width, float height, float depth, Color color, CTexture *textures) {
    setSize(width, height, depth);
    setTexture(textures);
    setColor(color);
    setRepetitionTexture(1);
    setScale(1.0f);
	enableRepetition();
}

Prisma::Prisma(float width, float height, float depth, Color color) {
    setSize(width, height, depth);
    setColor(color);
    setRepetitionTexture(1);
    setScale(1.0f);
	enableRepetition();
}

Prisma::Prisma(float width, float height, float depth): color() {
    setSize(width, height, depth);
    setColor(Color(1, 1, 1));
    setRepetitionTexture(1);
    setScale(1.0f);
	enableRepetition();
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
    if (t.GLindex != 0) {
        for (int i = 0; i < 6; ++i) {
            this->textures[i] = t;
        }
    }
}

void Prisma::setColor(Color color) {
    this->color = color;
}

void Prisma::draw() {
    this->x *= scale;
    this->y *= scale;
    this->z *= scale;
    Vertex v[8] = {
        Vertex(-x,  y,  z),
        Vertex( x,  y,  z),
        Vertex(-x, -y,  z),
        Vertex( x, -y,  z),
        Vertex(-x, -y, -z),
        Vertex( x, -y, -z),
        Vertex( x,  y, -z),
        Vertex(-x,  y, -z)
    };

    Plane p[6] = {
        Plane(v[0], v[1], v[6], v[7], textures[0]), // XZ Arriba
        Plane(v[4], v[5], v[3], v[2], textures[1]), // XZ Abajo
        Plane(v[0], v[2], v[3], v[1], textures[2]), // XY Z positiva
        Plane(v[6], v[5], v[4], v[7], textures[3]), // XY Z negativa
        Plane(v[1], v[3], v[5], v[6], textures[4]), // YZ en X positiva
        Plane(v[7], v[4], v[2], v[0], textures[5]) // YZ X negativa
    };
    for (size_t i = 0; i < 6; i++) {
		if (this->flagRepetition)
        	p[i].calculateRepetitions(repTexture, repTexture);
		else
			p[i].noRepetition();
        p[i].draw();
    }
}

void Prisma::draw(float offsetY) {
    this->x *= scale;
    this->y *= scale;
    this->z *= scale;
    Vertex v[8] = {
        Vertex(-x,  y,  z),
        Vertex( x,  y,  z),
        Vertex(-x, -y,  z),
        Vertex( x, -y,  z),
        Vertex(-x, -y, -z),
        Vertex( x, -y, -z),
        Vertex( x,  y, -z),
        Vertex(-x,  y, -z)
    };

    Plane p[6] = {
        Plane(v[0], v[1], v[6], v[7], textures[0]), // XZ Arriba
        Plane(v[4], v[5], v[3], v[2], textures[1]), // XZ Abajo
        Plane(v[0], v[2], v[3], v[1], textures[2]), // XY Z positiva
        Plane(v[6], v[5], v[4], v[7], textures[3]), // XY Z negativa
        Plane(v[1], v[3], v[5], v[6], textures[4]), // YZ en X positiva
        Plane(v[7], v[4], v[2], v[0], textures[5]) // YZ X negativa
    };
    for (size_t i = 0; i < 6; i++) {
        if (this->flagRepetition)
            p[i].calculateRepetitions(repTexture, repTexture);
        else
            p[i].noRepetition();
        p[i].draw(offsetY);
    }
}

void Prisma::drawSky(float anim) {
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
        glTexCoord2f(0.0f - anim, 0.0f); glVertex3fv(v[1]);
        glTexCoord2f(1.0f - anim, 0.0f); glVertex3fv(v[0]);
        glTexCoord2f(1.0f - anim, 1.0f); glVertex3fv(v[7]);
        glTexCoord2f(0.0f - anim, 1.0f); glVertex3fv(v[6]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[1].GLindex);
    //Plano inferior
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[3]);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(v[5]);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(v[4]);
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(v[2]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[2].GLindex);
    //Plano 1
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f + anim, 0.0f); glVertex3fv(v[3]);
        glTexCoord2f(1.0f + anim, 0.0f); glVertex3fv(v[2]);
        glTexCoord2f(1.0f + anim, 1.0f); glVertex3fv(v[0]);
        glTexCoord2f(0.0f + anim, 1.0f); glVertex3fv(v[1]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[3].GLindex);
    //Plano 2
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f + anim, 0.0f); glVertex3fv(v[5]);
        glTexCoord2f(1.0f + anim, 0.0f); glVertex3fv(v[3]);
        glTexCoord2f(1.0f + anim, 1.0f); glVertex3fv(v[1]);
        glTexCoord2f(0.0f + anim, 1.0f); glVertex3fv(v[6]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[4].GLindex);
    //Plano 3
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f + anim, 0.0f); glVertex3fv(v[4]);
        glTexCoord2f(1.0f + anim, 0.0f); glVertex3fv(v[5]);
        glTexCoord2f(1.0f + anim, 1.0f); glVertex3fv(v[6]);
        glTexCoord2f(0.0f + anim, 1.0f); glVertex3fv(v[7]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[5].GLindex);
    //Plano 4
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f + anim, 0.0f); glVertex3fv(v[2]);
        glTexCoord2f(1.0f + anim, 0.0f); glVertex3fv(v[4]);
        glTexCoord2f(1.0f + anim, 1.0f); glVertex3fv(v[7]);
        glTexCoord2f(0.0f + anim, 1.0f); glVertex3fv(v[0]);
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

void Prisma::setScale(float s) {
    if (this->scale >= 1.0) {
        this->scale = s;
    } else {
        this->scale = 1.0f;
    }
}

void Prisma::setRepetitionTexture(float rt) {
    this->repTexture = rt;
}

void Prisma::disableRepetition() {
	this->flagRepetition = false;
}

void Prisma::enableRepetition() {
	this->flagRepetition = true;
}
