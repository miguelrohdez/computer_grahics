/*
 * Semestre 2018-1
 * Alumnos:
 * Barriga Martinez Diego Alberto
 * Oropeza Vilchis Luis Alberto
 */

//#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include "Camera.h"
#include "TextureLoader.h"
#include "AnimationPlane.h"
#include "AnimationReloj.h"
#include "Elements.h"


ALCdevice  *device;
ALCcontext *context;
ALCenum error;
ALfloat listenerOri[] = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
ALuint source, buffer;
ALint source_state;

CCamera camara;
GLfloat g_lookupdown = 0.0f; // Posición en el eje Z

float animax = 0.0f;
bool sentido = true;

/*
 * KeyFrames
 */
AnimationReloj reloj;
AnimationPlane avion;

/*
 * Para usar texturas
 */
TextureLoader textures;

/*
 * Función que carga y da valores a las animaciones
 */
void loadKeyFrames() {
	reloj.loadData();
	avion.loadData();

	reloj.setTextures(textures.reloj, textures.pinturaNegra);
	avion.setTextures(textures.avionGris, textures.avionVerde);
	reloj.setActivate(true);
}


void checkError(ALenum e, int pos) {
	if(e != AL_NO_ERROR) {
		printf("Algo salió mal %d\n", pos);
	}
}

void freeAudio() {
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void initAL() {
	device = alcOpenDevice(NULL); // NULL implica el dispositivo por default
	// Abre dispositivo de audio
	if (!device) {
			printf("No se pudo abrir el dispositivo de audio.\n");
			exit(0);
	} else {
		printf("Dispotivio de audio abierto correctamente\n");
	}
	// Crea el contexto
	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		printf("Error al crear al contexto\n");
	}
	checkError(alGetError(), 0);

	// Genera la fuente
	alGenSources((ALuint)1, &source);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_TRUE);
	checkError(alGetError(), 4);

	// Inicializar Listener
	alListener3f(AL_POSITION, 0, 0, 00);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);
	checkError(alGetError(), 3);


	// Genera el buffer para el audio
	buffer = alutCreateBufferFromFile("../res/Sound/cowboy.wav");
	if ( alutGetError() != ALUT_ERROR_NO_ERROR ) {
		printf("Error al cargar el audio\n");
	}

	checkError(alGetError(), 5);
	//Ligando buffer con source
	alSourcei(source, AL_BUFFER, buffer);
	checkError(alGetError(), 6);
	alSourcePlay(source);

}

/*
 * Función para inicializar parámetros
 */
void initGL() {
	glClearColor(0.3f, 0.3f, 0.3f, 0.5f);                // Fondo negro
	glClearDepth(1.0f);                                    // Valor para el Depth Buffer
	glEnable(GL_DEPTH_TEST);                            // Activa Depth Testing
	glDepthFunc(GL_LEQUAL);                                // Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Correccion de cálculos de perspectiva
	camara.Position_Camera(50, 50, 10, 0, 50, 0, 0, 1, 0);
	textures.load();
	loadKeyFrames();
}


void dibujaEjes() {
	glBegin(GL_LINES);
		//Eje X
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1000.0f, 0.0f, 0.0f);
		glVertex3f(1000.0f, 0.0f, 0.0f);
		//Eje Y
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -1000.0f, 0.0f);
		glVertex3f(0.0f, 1000.0f, 0.0f);
		//Eje Z
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -1000.0f);
		glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
	glPointSize(10.0f);
	glBegin(GL_POINTS);
		//"Flecha" eje X
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1000.0f, 0.0f, 0.0f);
		//"Flecha" eje Y
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 1000.0f, 0.0f);
		//"Flecha" eje Z
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
	glPointSize(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void dibujarTerreno() {
	p.setColor(blanco);
	p.setRepetitionTexture(0.2);
	p.setTexture(textures.pasto4);	
	p.draw(2000, 5, 2000);	
}

void dibujarPiso() {
	p.setTexture(textures.tiles);
	p.draw(560, 1, 560);
}

void dibujarMesa() {
	p.setScale(4.0f);
	p.draw(8, 0.5, 3, textures.madera1);
	glTranslatef(0.0f, 0.75*2, 0.0f);
	p.draw(8.5, 0.25, 3.5, textures.pinturaBlanca2);
	glTranslatef(0.0f, 7.25f*2, 0.0f);
	p.draw(7.0f, 7.0f, 3.0f, textures.madera1);
	glTranslatef(0.0f, 7.2f*2, 0.0f);
	p.draw(8.5, 0.2, 3.5, textures.pinturaBlanca2);
	glTranslatef(0.0f, 0.4f*2, 0.6f*2);
	p.draw(15, 0.2, 10, textures.madera1);

	p.setScale(1.0);
}

/* Función que dibuja un sillon */
void dibujaSillon(float scale, CTexture sillonT=textures.sillon){

	// Base
	p.draw(20*scale, 3.5*scale, 10*scale, sillonT);
	// Recarga brazos
	glPushMatrix();
		glTranslatef(8.5*scale, 2.8*scale, 0);
		p.draw(2*scale, 2*scale, 10*scale);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-8.5*scale, 2.8*scale, 0);
		p.draw(2*scale, 2*scale, 10*scale);
	glPopMatrix();

	// Respaldo
	glPushMatrix();
		glTranslatef(0*scale, 3*scale, 4.3*scale);
		p.draw(15*scale, 3*scale, 1.5*scale);
	glPopMatrix();
}

/* Función que dibuja una cama*/
void dibujaCama(float scale){

	// Colchon
	p.draw(20*scale, 3*scale, 23*scale, textures.cobija);

	//Almohadas
	glPushMatrix();
		glTranslatef(5*scale, 1.5*scale, 9*scale);
		p.draw(8*scale, 0.7f*scale, 4*scale, textures.almohada);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-5*scale, 1.5*scale, 9*scale);
		p.draw(8*scale, 0.7f*scale, 4*scale, textures.almohada);
	glPopMatrix();

	//Cabecera
	glPushMatrix();
		glTranslatef(0*scale, 0.5*scale, 11.9*scale);
		p.draw(22*scale, 7*scale, 0.7f*scale, textures.madera1);
	glPopMatrix();

	//Base
	glPushMatrix();
		glTranslatef(0*scale, -2*scale, -0.1*scale);
		p.draw(21.2*scale, 3*scale, 23.2*scale, textures.madera);
	glPopMatrix();
}


void dibujaMuebleTv(float scale=1.0f){
	p.draw(20*scale, 15*scale, 7*scale,textures.madera1);
}

void dibujaTv(float scale=1.0f){
	// Base
	p.draw(5.5*scale, 0.2*scale, 0.3*scale, textures.pinturaNegra);

	// Base (cilindro)
	glPushMatrix();
		glTranslatef(0*scale, 0.05*scale, 0*scale);
		Cylinder b(0.3*scale, 1.5*scale, textures.pinturaNegra);
		b.draw();
	glPopMatrix();

	// Marco
	glPushMatrix();
		glTranslatef(0*scale, 5*scale, 0*scale);
		p.draw(15*scale, 7*scale, 0.5*scale);

		// Pantalla
		glPushMatrix();
			glTranslatef(0*scale, 0*scale, -0.2*scale);
			p.setRepetitionTexture(0.7f);
			p.draw(13*scale, 6*scale, 0.2*scale, textures.noise);
		glPopMatrix();
	glPopMatrix();
}

void dibujaTapete(float scale=1.0f){
	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, 0.1 );

	p.setRepetitionTexture(0.35f);
	p.draw(56*scale, 1*scale, 58*scale, textures.tapete);

	glDisable( GL_ALPHA_TEST );
}

void dibujaCarretera(){
	p.setRepetitionTexture(0.08f);
	glRotatef(90, 0, 1, 0);
	p.draw(250, 0.5, 2000, textures.road);
}

void dibujaGarage(){

	// Piso de piedra (Alberca)
	p.setRepetitionTexture(1.0f);
	p.draw(360, 1, 35, textures.camino);

	// Piso de piedra (Casa)
	glPushMatrix();
		glTranslatef(0, 0, -70);
		p.draw(360, 1, 35, textures.camino);
	glPopMatrix();

	// Adoquinado inferior
	glPushMatrix();
		glTranslatef(0, 0, -35);
		p.draw(360, 0.5, 156, textures.adoquin);
	glPopMatrix();
}

void dibujarCasa(float heightWall, float scale, float textureRep=0.8f) {
	float anchoBarda = 1.1f;

	p.setRepetitionTexture(textureRep);
	p.setTexture(textures.bardaA);
	p.setScale(scale);
	glPushMatrix(); // Bardas
		glTranslatef(-50 * scale, 0, 50 * scale); // A
		p.draw(30, heightWall, anchoBarda);
		p.setTexture(textures.bardaA);

		glTranslatef(-15 * scale, 0, -10 * scale); // B
		p.draw(anchoBarda, heightWall, 20, textures.bardaB);

		glTranslatef(30 * scale, 0, 0); // C
		p.draw(anchoBarda, heightWall, 20, textures.bardaC);

		glTranslatef(-20 * scale, 0, -10 * scale); // D
		p.draw(20, heightWall, anchoBarda, textures.bardaD);

		glTranslatef(75 * scale, 0, 0); // E
		p.draw(90, heightWall, anchoBarda, textures.bardaE);

		glTranslatef(-55 * scale, 0, -15 * scale); // F
		p.draw(anchoBarda, heightWall, 30, textures.bardaF);

		glTranslatef(-30 * scale, 0, -5 * scale); // G
		p.draw(anchoBarda, heightWall, 40, textures.bardaG);

		glTranslatef(130 * scale, 0, -5 * scale); // H
		p.draw(anchoBarda, heightWall, 50, textures.bardaH);

		glTranslatef(-85 * scale, 0, -5 * scale); // I
		p.draw(30, heightWall, anchoBarda, textures.bardaI);

		glTranslatef(-30 * scale, 0, -10 * scale); // J
		p.draw(30, heightWall, anchoBarda, textures.bardaJ);

		glTranslatef(55 * scale, 0, 0); // K
		p.draw(40, heightWall, anchoBarda, textures.bardaK);

		glTranslatef(-25 * scale, 0, -10 * scale); // L
		p.draw(10, heightWall, anchoBarda, textures.bardaL);

		glTranslatef(52.5 * scale, 0, 0); // M
		p.draw(15, heightWall, anchoBarda, textures.bardaM);

		glTranslatef(25 * scale, 0, 0); // N
		p.draw(15, heightWall, anchoBarda, textures.bardaN);

		glTranslatef(-122.5 * scale, 0, -10 * scale); // Ñ
		p.draw(anchoBarda, heightWall, 40, textures.bardaNE);

		glTranslatef(90 * scale, 0, -5 * scale); // O
		p.draw(anchoBarda, heightWall, 30, textures.bardaO);

		glTranslatef(-60 * scale, 0, 0); // P
		p.draw(anchoBarda, heightWall, 30, textures.bardaP);

		glTranslatef(20 * scale, 0, 0); // Q
		p.draw(anchoBarda, heightWall, 30, textures.bardaQ);

		glTranslatef(-35 * scale, 0, -15 * scale); // R
		p.draw(30, heightWall, anchoBarda, textures.bardaR);

		glTranslatef(25 * scale, 0, 0); // S
		p.draw(20, heightWall, anchoBarda, textures.bardaS);

		glTranslatef(30 * scale, 0, 0); // T
		p.draw(40, heightWall, anchoBarda, textures.bardaT);

		glTranslatef(20 * scale, 0, 35 * scale); // U
		p.draw(anchoBarda, heightWall, 10, textures.bardaU);
	glPopMatrix();

	//Secciones de Techo
	heightWall += 0.55; // Se suma ancho del techo
	glPushMatrix();
		glTranslatef(-50 * scale, heightWall*(scale/2), 40 * scale); // AT
		p.draw(30, anchoBarda, 20, textures.techoA);
		glTranslatef(30 * scale, 0, -25 * scale); // BT
		p.draw(30, anchoBarda, 30, textures.techoB);
		glTranslatef(-30 * scale, 0, -5 * scale); // CT
		p.draw(30, anchoBarda, 40, textures.techoC);
		glTranslatef(60 * scale, 0, 0); // DT
		p.draw(30, anchoBarda, 40, textures.techoD);
		glTranslatef(35 * scale, 0, -5 * scale); // ET
		p.draw(40, anchoBarda, 50, textures.techoE);
		glTranslatef(-55 * scale, 0, -10 * scale); // FT
		p.draw(10, anchoBarda, 10, textures.techoF);
		glTranslatef(-15 * scale, 0, -5 * scale); // GT
		p.draw(20, anchoBarda, 20, textures.techoG);
		glTranslatef(-25 * scale, 0, -20 * scale); // HT
		p.draw(30, anchoBarda, 40, textures.techoH);
		glTranslatef(55 * scale, 0, 0); // IT
		p.draw(40, anchoBarda, 40, textures.techoI);
		glTranslatef(-30 * scale, 0, -5 * scale); // JT
		p.draw(20, anchoBarda, 30, textures.techoJ);
	glPopMatrix();
	heightWall -= 5.55;
	anchoBarda = 0.5;
	p.disableRepetition();
	glTranslatef(0, -2.5 * scale, 0);
	glPushMatrix();
		glTranslatef(-40.25 * scale, 0, 30 * scale); // P1
		p.draw(9.75, heightWall, anchoBarda, textures.door1);
		glTranslatef(10.25 * scale, 0, 0); // P2
		p.draw(10, heightWall, anchoBarda, textures.door2);
		glTranslatef(-5 * scale, 0, -34.75 * scale); // P3
		p.draw(anchoBarda, heightWall, 9.35, textures.door3);
		glTranslatef(0, 0, -10.5 * scale); // P4
		p.draw(anchoBarda, heightWall, 9.35, textures.door3);
		glTranslatef(20 * scale, 0, 0); // P5
		p.draw(anchoBarda, heightWall, 9.6, textures.door4);
		glTranslatef(-14.5 * scale, 0, -5 * scale); // P6
		p.draw(9.5, heightWall, anchoBarda, textures.door5);
		glTranslatef(75 * scale, 0, 0); // P7
		p.draw(11, heightWall, anchoBarda, textures.door2);
	glPopMatrix();
	p.enableRepetition();
	heightWall = 5;
	anchoBarda = 1.1;
	glTranslatef(0, 12.5 * scale, 0);
	glPushMatrix();
		glTranslatef(-40.25 * scale, 0, 30 * scale); // P1
		p.draw(9.75, heightWall, anchoBarda, textures.bardaD);
		glTranslatef(10.25 * scale, 0, 0); // P2
		p.draw(10, heightWall, anchoBarda, textures.bardaE);
		glTranslatef(-5 * scale, 0, -34.75 * scale); // P3
		p.draw(anchoBarda, heightWall, 9.35, textures.bardaF);
		glTranslatef(0, 0, -10.5 * scale); // P4
		p.draw(anchoBarda, heightWall, 9.35, textures.bardaJ);
		glTranslatef(20 * scale, 0, 0); // P5
		p.draw(anchoBarda, heightWall, 9.6, textures.bardaK);
		glTranslatef(-14.5 * scale, 0, -5 * scale); // P6
		p.draw(9.5, heightWall, anchoBarda, textures.bardaL);
		glTranslatef(75 * scale, 0, 0); // P7
		p.draw(11, heightWall, anchoBarda, textures.bardaM);
	glPopMatrix();
	p.setScale(1.0);

	// Ventanas
	// Viendo de frente
	// Lado izquierdo
	glPushMatrix();
		glTranslatef(260, -35, 20);
		glRotatef(90, 0, 1, 0);
		p.disableRepetition();
		p.draw(50, 50, 5, textures.ventana);
		p.enableRepetition();
	glPopMatrix();

	 // Frontal 1
	glPushMatrix();
		glTranslatef(30, -35, -200);	
		p.disableRepetition();
		p.draw(50, 50, 5, textures.ventana);
		p.enableRepetition();
	glPopMatrix();

	// Frontal 2
	glPushMatrix();
		glTranslatef(-180, -35, -200);	
		p.disableRepetition();
		p.draw(50, 50, 5, textures.ventana);
		p.enableRepetition();
	glPopMatrix();
}

void dibujarSkyBox() {
	p.setSize(2000, 1000, 2000);
	p.setColor(blanco);
	p.setTexture(textures.cielo);
	p.drawSky(animax);
}

/* Función para depurar los planos */
void dibujarPlano() {
	Vertex v1 = Vertex(5,0,0);
	Vertex v2 = Vertex(0,0,0);
	Vertex v3 = Vertex(0,10,0);
	Vertex v4 = Vertex(5,10 ,0);
	Plane p = Plane(v1, v2, v3, v4, textures.cuadroDebug);
	p.draw();
}

void dibujaLavabo(float scale) {
	p.setScale(scale);
	p.disableRepetition();
	p.draw(4, 1, 5, textures.lavaboComplete);
	p.enableRepetition();
	p.setScale(1.0f);
}

void testCylinder() {
	Cylinder c(10, 10, textures.cuadroDebug);
	c.drawHalf();
}

void testCone() {
	Cone c(10, 10, textures.cuadroDebug);
	c.draw();
}

void dibujarRotoplas(float scale) {
	c.draw(4.5*scale, 7*scale, textures.rotoplas);
	glTranslatef(0*scale, 7*scale, 0*scale);
	c.draw(4.5*scale, 2.5*scale, 2*scale, textures.pinturaNegra);
}

void dibujaTocador(float scale=1.0f){
	p.setColor(cafeMesa);
	p.draw(45*scale, 15*scale, 8*scale, textures.madera1);
	p.setColor(blanco);
}

void dibujaAlberca(float scale=1.0f){
	p.disableRepetition();
	p.setColor(blanco);
	p.draw(70*scale, 10*scale, 100*scale, textures.agua);
	p.enableRepetition();
	// Agua
	glPushMatrix();
		glTranslatef(0*scale, -5*scale, 50*scale);
		c.drawHalf(35*scale, 10*scale, textures.bathroom1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0*scale, -5*scale, -50*scale);
		glRotatef(180, 0, 1, 0);
		c.drawHalf(35*scale, 10*scale, textures.bathroom1);
	glPopMatrix();

	// Base exterior
	glPushMatrix();
		glTranslatef(0*scale, 2*scale, 0*scale);
		p.draw(150, 8, 280, textures.poolFloor);
	glPopMatrix();

	// Camino empedrado
	glPushMatrix();
		glTranslatef(170, 0, -15);
		p.draw(200, 2, 50, textures.camino);
	glPopMatrix();
}

void dibujarRetrete() {
	c.draw(5, 8, 20, textures.pinturaBlanca2);
	glTranslatef(0, 10, 8);
	p.draw(12, 16, 16, textures.pinturaBlanca2);
	glTranslatef(0, 8, 13);
	p.draw(30, 16, 10);
}

void dibujaSilla(float scale) {
	p.setScale(scale);
	p.draw(0.5, 5, 0.5, textures.madera1);
	glTranslatef(4 * scale, 0, 0);
	p.draw(0.5, 5, 0.5, textures.madera1);
	glTranslatef(0, 0, 4 * scale);
	p.draw(0.5, 5, 0.5, textures.madera1);
	glTranslatef(-4 * scale, 0, 0);
	p.draw(0.5, 5, 0.5, textures.madera1);
	glTranslatef(2 * scale, 2.75 * scale, -2 * scale);
	p.draw(4.5, 0.5, 4.5);
	glTranslatef(-1.75 * scale, 2.75 * scale, 2 * scale);
	p.draw(1, 5, 0.5);
	glTranslatef(3.5 * scale, 0 , 0);
	p.draw(1, 5, 0.5);
	glTranslatef(-1.75 * scale, 2 * scale, 0);
	p.draw(2.5, 1, 0.5);
	glTranslatef(0, -1.75 * scale, 0);
	p.draw(2.5, 1, 0.5);
	glTranslatef(0, -1.75 * scale, 0);
	p.draw(2.5, 1, 0.5);
	p.setScale(1.0f);
}

void dibujarMesaCristal(float scale) {
	float hb = 0.5f * scale; // Altura base 5 cm
	float rb = 1.5f * scale; // Radio Base 0.3m
	float ht = 4.5f * scale; // Altura tubo 1m
	float rt = 0.5f * scale; // Radio tubo 5cm
	float hc = 0.5f * scale; // Ancho Cristal 10cm
	float rc = 5.0f * scale; // Radio cristal 0.9m

	Cylinder c(rb, hb, textures.metal1);
	c.draw();
	glTranslatef(0, hb, 0);
	c.draw(rt, ht);
	glTranslatef(0, ht, 0);
	c.draw(rc, hc, textures.cristalBlanco);
}

void dibujarEstante(float scale) {
	float alto = 0.2f * scale; // Alto de las piezas de madera 2cm
	float ancho = 3.0f * scale; // Ancho de las piezas de madera 30cm
	float largo = 3.0f * scale; // Largo de las piezas 30cm
	float d1 = 3.0f * scale; // distancia vertical entre piezas 30cm
	float altoCostado = 3 * d1 + 4 * alto;
	float anchoCostado = 0.2f * scale;

	Prisma p(ancho, alto, largo);
	p.setTexture(textures.madera);
	p.draw();
	glTranslatef(0, alto + d1, 0);
	p.draw(ancho, alto, largo);
	glTranslatef(0, alto + d1, 0);
	p.draw(ancho, alto, largo);
	glTranslatef(0, alto + d1, 0);
	p.draw(ancho, alto, largo);
	glTranslatef((ancho + anchoCostado) / 2, -(altoCostado - alto) / 2, 0);
	p.draw(anchoCostado, altoCostado, largo);
	glTranslatef(-(anchoCostado + ancho), 0, 0);
	p.draw(anchoCostado, altoCostado, largo);
	glTranslatef((ancho + anchoCostado) / 2, 0, -(largo + anchoCostado) / 2);
	p.draw(ancho + (anchoCostado * 2), altoCostado, anchoCostado);
}

/*
 * Función que dibuja
 */
void display(void) {
	//alSourcePlay(source);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //TODO: Entender que hace
	glLoadIdentity();
	glRotatef(g_lookupdown,1.0f, 0, 0);
	gluLookAt(camara.mPos.x, camara.mPos.y, camara.mPos.z,
			  camara.mView.x, camara.mView.y, camara.mView.z,
			  camara.mUp.x, camara.mUp.y, camara.mUp.z);

	dibujaEjes();
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); // Skybox
		glTranslatef(0,450,0);
		dibujarSkyBox(animax);
	glPopMatrix();

	glPushMatrix(); // Pasto
		dibujarTerreno();
	glPopMatrix();

	glTranslatef(0, 2.5, 0); // Eleva a la altura del pasto
	glPushMatrix(); // Piso casa
		dibujarPiso();
	glPopMatrix();

	glTranslatef(0, 0.5, 0); //Eleva a la altura del piso
	glPushMatrix(); // Paredes
		glTranslatef(0, 50, 0);
		dibujarCasa(25.0, 4.0);
	glPopMatrix();

	glPushMatrix(); // Mesa madera
		glTranslatef(0, 0, 60);
		dibujarMesa();
	glPopMatrix();

	// Sección de la sala
	glPushMatrix();
		glTranslatef(200, 5.25, -10);
		glRotatef(180, 0, 1, 0);
		dibujaSillon(4.5f, textures.sillon1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(120, 5.25, 60);
		glRotatef(-90, 0, 1, 0);
		dibujaSillon(4.0f, textures.sillon1);	
	glPopMatrix();

	glPushMatrix();
		glTranslatef(240, 10, 100);
		glRotatef(40, 0, 1, 0);
		dibujaMuebleTv(1.4f);
		glPushMatrix();
			glTranslatef(0, 10.5, 0);
			dibujaTv(3.0f);
		glPopMatrix();
	glPopMatrix();

	glPushMatrix(); // Tapete
		glTranslatef(200, 2, 70);
		dibujaTapete();
	glPopMatrix();


	// Sección del techo
	glPushMatrix();
		glTranslatef(-50, 105, -70);
		dibujarRotoplas(4.0f);
	glPopMatrix();


	// Sección de recamara amarilla
	glPushMatrix();
		glTranslatef(35, 12, -150);
		glRotatef(180, 0, 1, 0);
		dibujaCama(4.0f);
	glPopMatrix();

	glPushMatrix(); // Tocador
		glTranslatef(-48, 15, -140);
		glRotatef(90, 0, 1, 0);
		dibujaTocador(2.0f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35, 15, -50);
		dibujaMuebleTv(2.0f);
		glPushMatrix();
			glTranslatef(0, 15, 0);
			dibujaTv(4.0f);
		glPopMatrix();
	glPopMatrix();

	// Alberca
	glPushMatrix();
		glTranslatef(-100, 0, 550);
		glRotatef(90, 0, 1, 0);
		dibujaAlberca(1.5f);
	glPopMatrix();

	// Carretera
	glPushMatrix();
		glTranslatef(0, 0, -700);
		dibujaCarretera();
	glPopMatrix();

	// Garage
	glPushMatrix();
		glTranslatef(100, 0, 236);
		dibujaGarage();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(90, 70, -37.75);
		reloj.draw();
	glPopMatrix();
	glPushMatrix(); // Retretes
		glTranslatef(-100, 0, -170);
		glRotatef(180, 0, 1, 0);
		dibujarRetrete();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-230, 0, 160);
		glRotatef(90, 0, -1, 0);
		dibujarRetrete();
	glPopMatrix();
	glPushMatrix(); // Lavabos
		glTranslatef(-130, 45, -140);
		dibujaLavabo(4.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-200, 40, 190);
		glRotatef(90, 0, 1, 0);
		dibujaLavabo(4);
	glPopMatrix();
	glPushMatrix(); // Sillas
		glTranslatef(-25, 10, 80);
		dibujaSilla(4.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(5, 10, 80);
		dibujaSilla(4.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(25, 10, 40);
		glRotatef(180, 0, 1, 0);
		dibujaSilla(4.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-5, 10, 40);
		glRotatef(180, 0, 1, 0);
		dibujaSilla(4.0f);
	glPopMatrix();
	glPushMatrix(); // Cuarto Azul
		glTranslatef(-220, 10, 75);
		dibujaCama(3.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-240, 10, -60);
		glRotatef(-40, 0, 1, 0);
		dibujaMuebleTv(1.4f);
		glPushMatrix();
			glTranslatef(0, 10.5, 0);
			dibujaTv(3.0f);
		glPopMatrix();
	glPopMatrix();
	glPushMatrix(); // Cuarto
		glTranslatef(-180, 10, -160);
		glRotatef(180, 0, 1, 0);
		dibujaCama(3.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-205, 10, -30);
		glRotatef(180, 0, 1, 0);
		dibujaMuebleTv(1.4f);
		glPushMatrix();
			glTranslatef(0, 10.5, 0);
			dibujaTv(3.0f);
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
		avion.draw();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-250, 0.1, -188);
		glRotatef(90, 0, 1, 0);
		dibujarEstante(4.0f);
	glPopMatrix();
	glEnable(GL_BLEND); // Figuras con opacidad
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		glTranslatef(200, 2, 70);
		dibujarMesaCristal(4.0f);
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void animation() {

	if(sentido){
		animax += 0.0001;
		if(animax >= 1.0){
			printf("En sentido true cambio a false\n");
			sentido = false;
		}
	}
	else if(sentido == false){
		animax -= 0.0001;
		if(animax <= 1.0){
			printf("En sentido false cambio a true\n");
			sentido = true;
		}
	}

	reloj.update();
	avion.update();
	glutPostRedisplay();
}

/*
* Funcion para el reajuste de dibujo en ventana
*/
void reshape(int width, int height) {
	float relation;
	height = (height == 0) ? 1 : height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION); // Seleccionamos Projection Matrix
	glLoadIdentity();
	relation = (float)width / (float)height;
	gluPerspective(20 * relation, relation, 1.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW); // Seleccionamos Modelview Matrix
	glLoadIdentity();
}


/*
* Función que maneja el teclado
*/
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':   //Movimientos de camara
		case 'W':
			camara.Move_Camera( CAMERASPEED );
			break;
		case 's':
		case 'S':
			camara.Move_Camera(-(CAMERASPEED));
			break;
		case 'a':
		case 'A':
			camara.Strafe_Camera(-(CAMERASPEED));
			break;
		case 'd':
		case 'D':
			camara.Strafe_Camera( CAMERASPEED);
			break;
		case 'e':
		case 'E':
			camara.UpDown_Camera(CAMERASPEED);
			break;
		case 'q':
		case 'Q':
			camara.UpDown_Camera(-CAMERASPEED);
			break;
		/* Temporal para keyframes */
		case 'z':
			//reloj.left();
			break;
		case 'Z':
			//reloj.right();
			break;
		case 'x':
			//reloj.fordward();
			break;
		case 'X':
			//reloj.backward();
			break;
		case 'c':
			//reloj.up();
			break;
		case 'C':
			//reloj.down();
			break;
		case 'v':
			reloj.rotateManecilla();
			break;
		case 'V':
			//reloj.rotateXNegative();
			break;
		case 'M':
		case 'm':
			//reloj.saveToFile();
			break;
		case 'n':
		case 'N':
			//reloj.saveKeyframe();
			break;
		case 'b':
			avion.setActivate(true);
			break;
		case 'B':
			//reloj.setActivate(false);
			break;
		case 27:        // Cuando Esc es presionado...
			alutExit();
			freeAudio();
			exit(0);   // Salimos del programa
			break;
		default:        // Cualquier otra
			break;
	}
	glutPostRedisplay();
}

/*
* Función para el manejo de teclas especiales
*/
void arrowKeys(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
			g_lookupdown -= 1.0f;
			break;
		case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
			g_lookupdown += 1.0f;
			break;
		case GLUT_KEY_LEFT:
			camara.Rotate_View(-CAMERASPEED);
			break;
		case GLUT_KEY_RIGHT:
			camara.Rotate_View( CAMERASPEED);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

/*
* Función principal
*/
int main(int argc, char **argv) {
	glutInit(&argc, argv); // Inicializamos OpenGL
	alutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode (Clores RGB y alpha | Buffer Sencillo )
	glutInitWindowSize(1920, 1080); // Tamaño de la Ventana
	glutInitWindowPosition(-1, -1); //Posicion de la Ventana
	glutCreateWindow("Complejo Residencial"); // Nombre de la Ventana
	initGL(); // Parametros iniciales de la aplicacion
	initAL();
	glutDisplayFunc(display); // Función de dibujo
	glutReshapeFunc(reshape); // Función en caso de cambio de tamano
	glutKeyboardFunc(keyboard); // Función de manejo de teclado
	glutSpecialFunc(arrowKeys); // Función manejo teclas especiales
	glutIdleFunc(animation);
	glutMainLoop();
	return 0;
}
