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
#include "AnimationRocket.h"
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
AnimationRocket rocket;

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
	rocket.loadData();

	reloj.setTextures(textures.reloj, textures.pinturaNegra);
	avion.setTextures(textures.avionGris, textures.avionVerde);
	rocket.setTextures(textures.spaceX, textures.propulsor, textures.pinturaNegra);
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
		printf("Dispositivo de audio abierto correctamente\n");
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
	buffer = alutCreateBufferFromFile("../res/Sound/jazz_piano.wav");
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
		dibujaSillon(4.5f, textures.sillon);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(120, 5.25, 60);
		glRotatef(-90, 0, 1, 0);
		dibujaSillon(4.0f, textures.sillon);	
	glPopMatrix();

	glPushMatrix();
		glTranslatef(240, 10, 100);
		glRotatef(40, 0, 1, 0);
		dibujaMuebleTv(1.4f);
		glPushMatrix();
			glTranslatef(0, 10.5, 0);
			dibujaTv(3.0f, textures.ring);
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
		dibujaCama(4.0f, textures.cobija2);
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
			dibujaTv(4.0f, textures.ring2);
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

	// Cuarto Rojo
	glPushMatrix(); 
		glTranslatef(-220, 10, 75);
		dibujaCama(3.0f, textures.cobija);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-240, 10, -60);
		glRotatef(-40, 0, 1, 0);
		dibujaMuebleTv(1.4f);
		glPushMatrix();
			glTranslatef(0, 10.5, 0);
			dibujaTv(3.0f, textures.ring2);
		glPopMatrix();
	glPopMatrix();

	// Cuarto Floreado
	glPushMatrix(); 
		glTranslatef(-180, 10, -160);
		glRotatef(180, 0, 1, 0);
		dibujaCama(3.0f, textures.cobija2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-205, 10, -30);
		glRotatef(180, 0, 1, 0);
		dibujaMuebleTv(1.4f);
		glPushMatrix();
			glTranslatef(0, 10.5, 0);
			dibujaTv(3.0f, textures.ring);
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
		avion.draw();
	glPopMatrix();

	glPushMatrix();
		rocket.draw();
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

/*
 * Función de animación 
 */
void animation() {

	if(sentido){
		animax += 0.00001;
		if(animax >= 1.0){
			printf("En sentido true cambio a false\n");
			sentido = false;
		}
	}
	else if(sentido == false){
		animax -= 0.00001;
		if(animax <= -1.0){
			printf("En sentido false cambio a true\n");
			sentido = true;
		}
	}

	reloj.update();
	avion.update();
	rocket.update();
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
			rocket.left();
			break;
		case 'Z':
			rocket.right();
			break;
		case 'x':
			rocket.fordward();
			break;
		case 'X':
			rocket.backward();
			break;
		case 'c':
			rocket.up();
			break;
		case 'C':
			rocket.down();
			break;
		case 'v':
			rocket.rotateXPositive();
			break;
		case 'V':
			rocket.rotateXNegative();
			break;
		case 'M':
		case 'm':
			rocket.saveToFile();
			break;
		case 'n':
		case 'N':
			rocket.saveKeyframe();
			break;
		case 'b':
			rocket.setActivate(true);
			break;
		case 'B':
			rocket.setActivate(false);
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
