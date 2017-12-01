/*
 * Semestre 2018-1
 * Alumnos:
 * Barriga Martinez Diego Alberto
 * Oropeza Vilchis Luis Alberto
 */

#include <GL/freeglut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <AL/alut.h>
#include "Audio.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "AnimationPlane.h"
#include "AnimationReloj.h"
#include "AnimationRocket.h"
#include "AnimationFridge.h"
#include "AnimationBall.h"
#include "Elements.h"
#include "Sphere.h"

ALCdevice  *device;
ALCcontext *context;
ALCenum error;
ALfloat listenerOri[] = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
ALuint source, buffer;
ALint source_state;

CCamera camara, auxCamera;
GLfloat g_lookupdown = 0.0f; // Posición en el eje Z
GLfloat g_lookaux = 0.0f; // Variable auxiliar para auxCamera

/*
 * Para animar SkyBox
 */
float animax = 0.0f, flamaOffset = 0.0f;
bool bandera = false, techoBan = true;

/*
* Para cargar y usar audio
*/
Audio music;

/*
 * KeyFrames
 */
AnimationReloj reloj;
AnimationPlane avion;
AnimationPlane avionCasa;
AnimationRocket rocket;

/*
 * Máquinas de estado
 */
AnimationFridge refri;
AnimationBall ball[4];

/*
 * Para usar texturas
 */
TextureLoader textures;


/*
 * Función que carga y da valores a las animaciones
 */
void loadKeyFrames() {
	reloj.loadData();
	avion.loadData(FILE_KEYFRAME_PLANE);
	avionCasa.loadData(FILE_KEYFRAME_PLANECASA);
	rocket.loadData();

	reloj.setTextures(textures.reloj, textures.pinturaNegra);
	avion.setTextures(textures.avionGris, textures.avionVerde);
	avionCasa.setTextures(textures.cielo, textures.avionVerde);
	rocket.setTextures(textures.spaceX, textures.propulsor, textures.pinturaNegra, textures.explotion);
	reloj.setActivate(true);
}

void loadStateMachines() {
	refri = AnimationFridge();
	for (size_t i = 0; i < 4; i++) {
		ball[i] = AnimationBall();
	}
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
	loadStateMachines();
	music.play();
}

/*
 * Función que dibuja
 */
void display(void) {
	//alSourcePlay(source);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //TODO: Entender que hace
	glLoadIdentity();

	if(bandera){
		glRotatef(g_lookaux, 1.0f, 0, 0);
		gluLookAt(auxCamera.mPos.x, auxCamera.mPos.y, auxCamera.mPos.z,
			  auxCamera.mView.x, auxCamera.mView.y, auxCamera.mView.z,
			  auxCamera.mUp.x, auxCamera.mUp.y, auxCamera.mUp.z);
	}else{
		glRotatef(g_lookupdown, 1.0f, 0, 0);
		gluLookAt(camara.mPos.x, camara.mPos.y, camara.mPos.z,
			  camara.mView.x, camara.mView.y, camara.mView.z,
			  camara.mUp.x, camara.mUp.y, camara.mUp.z);
	}

	dibujaEjes();
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); // Skybox
		glTranslatef(0, 450, 0);
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
		dibujarCasa(25.0, 4.0, techoBan);
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
		avionCasa.draw();
	glPopMatrix();

	glPushMatrix();
		rocket.draw(flamaOffset);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-250, 0.1, -188);
		glRotatef(90, 0, 1, 0);
		dibujarEstante(4.0f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-125, 20, 18);
		dibujarEstufa(4.0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-113, 20, 35);
		refri.draw(4.0f, textures.crefri, textures.crefriPuerta, textures.crefriPuertaNevera);
	glPopMatrix();

	glEnable(GL_BLEND); // Figuras con opacidad
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		glTranslatef(-100, 0, 550);
		ball[0].draw(3.0f, textures.burbuja, -1, -1);
		glTranslatef(50, 0, 0);
		ball[1].draw(3.0f, textures.burbuja, -1, 1);
		glTranslatef(0, 0, 50);
		ball[2].draw(3.0f, textures.burbuja, 1, -1);
		glTranslatef(-50, 0, -50);
		ball[3].draw(3.0f, textures.burbuja, 1, 1);
	glPopMatrix();

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
	animax += 0.00009;
	flamaOffset += 0.175;
	reloj.update();
	avion.update();
	avionCasa.update();
	rocket.update();
	refri.update();
	for (size_t i = 0; i < 4; i++) {
		ball[i].update();
	}
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
			bandera = false;
			techoBan = true;
			camara.Move_Camera( CAMERASPEED );
			break;
		case 's':
		case 'S':
			bandera = false;
			techoBan = true;
			camara.Move_Camera(-(CAMERASPEED));
			break;
		case 'a':
		case 'A':
			bandera = false;
			techoBan = true;
			camara.Strafe_Camera(-(CAMERASPEED));
			break;
		case 'd':
		case 'D':
			bandera = false;
			techoBan = true;
			camara.Strafe_Camera( CAMERASPEED);
			break;
		case 'e':
		case 'E':
			bandera = false;
			techoBan = true;
			camara.UpDown_Camera(CAMERASPEED);
			break;
		case 'q':
		case 'Q':
			bandera = false;
			techoBan = true;
			camara.UpDown_Camera(-CAMERASPEED);
			break;
		case 'T':
			techoBan = true;
			bandera = false;
			break;
		case 't':
			auxCamera.Position_Camera(0, 900, 0, 0, 0, 0, 0, 0, 1);
			techoBan = false;
			bandera = true;
			break;
		case 'p':
		case 'P':
			for (size_t i = 0; i < 10; i++) {
				ball[i].activate();
			}
			break;
		/* Temporal para keyframes */

		case 'z':
			avionCasa.left();
			break;
		case 'Z':
			avionCasa.right();
			break;
		case 'x':
			avionCasa.fordward();
			break;
		case 'X':
			avionCasa.backward();
			break;
		case 'c':
			avionCasa.up();
			break;
		case 'C':
			avionCasa.down();
			break;
		case 'v':
			avionCasa.rotateXPositive();
			break;
		case 'V':
			avionCasa.rotateXNegative();
			break;
		case 'M':
		case 'm':
			avionCasa.saveToFile(FILE_KEYFRAME_PLANECASA);
			break;
		case 'n':
		case 'N':
			avionCasa.saveKeyframe();
			break;
		case 'b':
			rocket.setActivate(true);
			break;
		case 'B':
			rocket.setActivate(false);
			break;
		case 'h':
			avion.setActivate(true);
			break;
		case 'H':
			avion.setActivate(false);
			break;
		case 'i':
			avionCasa.setActivate(true);
			break;
		case 'I':
			avionCasa.setActivate(false);
			break;
		case 'r':
			refri.activateDoor1();
			break;
		case 'R':
			refri.activateDoor2();
			break;
		case '1':
			// Animación del Avión
			auxCamera.Position_Camera(-650, 800, -750, 40, 700, -750, 0, 1, 0);
			bandera = true;
			avion.setActivate(true);
			break;
		case '2':
			// Alberca
			auxCamera.Position_Camera(-100, 400, 700, -100, 0, 500, 0, 1, 0);
			bandera = true;
			break;
		case '3':
			// Animación Cohéte
			auxCamera.Position_Camera(800, 50, 0, 100, 150, 800, 0, 1, 0);
			bandera = true;
			rocket.setActivate(true);
			break;
		case '4':
			// Animación Reloj
			auxCamera.Position_Camera(90, 70, 0, 100, 70, -100, 0, 1, 0);
			bandera = true;
			break;
		case '5':
			// Animación refrigerador
			auxCamera.Position_Camera(-30, 50, 30, -50, 47, 30, 0, 1, 0);
			bandera = true;
			break;
		case '6':
			// Animación Avion casa
			auxCamera.Position_Camera(-650, 80, -750, 40, 80, -750, 0, 1, 0);
			bandera	= true;
			avionCasa.setActivate(true);
			break;
		case 27:        // Cuando Esc es presionado...
			alutExit();
			music.deleteAudio();
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
			if(bandera)
				g_lookaux -= 1.0f;
			else
				g_lookupdown -= 1.0f;
			break;
		case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
			if(bandera)
				g_lookaux += 1.0f;
			else
				g_lookupdown += 1.0f;
			break;
		case GLUT_KEY_LEFT:
			if(bandera)
				auxCamera.Rotate_View(-CAMERASPEED);
			else
				camara.Rotate_View(-CAMERASPEED);
			break;
		case GLUT_KEY_RIGHT:
			if(bandera)
				auxCamera.Rotate_View(CAMERASPEED);
			else
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
	music.loadData();
	initGL(); // Parametros iniciales de la aplicacion
	glutDisplayFunc(display); // Función de dibujo
	glutReshapeFunc(reshape); // Función en caso de cambio de tamano
	glutKeyboardFunc(keyboard); // Función de manejo de teclado
	glutSpecialFunc(arrowKeys); // Función manejo teclas especiales
	glutIdleFunc(animation);
	glutMainLoop();
	return 0;
}
